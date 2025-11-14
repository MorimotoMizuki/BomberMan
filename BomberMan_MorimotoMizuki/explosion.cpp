#include"obj.h"
#include"function.h"
#include"map.h"

CExplosion::CExplosion(Point p, MapPoint bombP, int bombLevel)
{
	LoadDivGraph("image\\explosion.png", EXPLOSION_IMG_NUM, 4, 3, IMGSIZE32, IMGSIZE32, ExplosionImgHandle);

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	pos = p;

	//爆弾の座標
	BombPos = bombP;

	//爆弾レベル
	BombLevel = bombLevel;

	ID = Obj_Id::EXPLOSION;
	pri = Pri_Id::pEXPLOSION;
}

int CExplosion::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);

	//プレイヤーの座標がスクロールする座標になった場合
	if (p->pos.x == DRAW_CHIP_W * CHIP_SIZE / 2)
	{
		//プレイヤーの移動と逆方向に移動
		pos.x -= p->vec.x;
	}

	//当たり判定
	HitAction(base);

	//爆発アニメーション処理
	if (!ExplosionAnim(EXPLOSION_ANIM_NUM, &AnimIndex))
		FLAG = false;

	return 0;
}

void CExplosion::Draw()
{
	//画像描画
	//中央
	DrawExtendGraph(pos.x, pos.y, pos.x + ImgWidth, pos.y + ImgHeight, ExplosionImgHandle[EXPLOSION_ANIM_ORDER[AnimIndex]], true);

	//上,下,左,右
	ExplosionPointData[0] = DrawExplosion( 0, -1	, BombLevel, ExplosionEffectId::VERTICAL);
	ExplosionPointData[1] = DrawExplosion( 0,  1	, BombLevel, ExplosionEffectId::VERTICAL);
	ExplosionPointData[2] = DrawExplosion(-1,  0	, BombLevel, ExplosionEffectId::HORIZONTAL);
	ExplosionPointData[3] = DrawExplosion( 1,  0	, BombLevel, ExplosionEffectId::HORIZONTAL);

	//Point startPos = { pos.x + ImgWidth / 2 ,pos.y + ImgHeight / 2 };
	//for (int i = 0; i < 4; i++){
	//	Point expPos = { std::get<1>(ExplosionPointData[i]).x * std::get<2>(ExplosionPointData[i]) * 64,
	//					 std::get<1>(ExplosionPointData[i]).y * std::get<2>(ExplosionPointData[i]) * 64
	//	};
	//	DrawBox(startPos.x, startPos.y, startPos.x + expPos.x, startPos.y + expPos.y, GetColor(255, 0, 0), false);
	//}

}

CExplosion::~CExplosion()
{

}

//当たり判定
void CExplosion::HitAction(vector<unique_ptr<BaseVector>>& base)
{
	if (std::get<0>(ExplosionPointData[0]).x == 0)
		return;

	bool isBreak{ false };	
	for (int i = 0; i < base.size(); i++)
	{
		//ブロックオブジェクトと判定
		if (base[i]->ID == BLOCK)
		{
			if (((CBlock*)base[i].get())->tipNo == 1)
			{
				Point blockPos = base[i].get()->pos;
				MapPoint systemBlockPos = ((CBlock*)base[i].get())->SystemPos;

				for (int i = 0; i < 4; i++)
				{
					Point startPos = { pos.x + std::get<0>(ExplosionPointData[i]).x ,pos.y + std::get<0>(ExplosionPointData[i]).y };
					Point expPos = { std::get<1>(ExplosionPointData[i]).x * std::get<2>(ExplosionPointData[i]) * 32,
									 std::get<1>(ExplosionPointData[i]).y * std::get<2>(ExplosionPointData[i]) * 32
					};

					//当たり判定
					if (blockPos.x < startPos.x + expPos.x && blockPos.x + 64 > startPos.x + expPos.x &&
						blockPos.y - WINDOW_HEADER < startPos.y + expPos.y - WINDOW_HEADER && blockPos.y - WINDOW_HEADER + 64 > startPos.y + expPos.y - WINDOW_HEADER)
					{
						if (gNowMap[systemBlockPos.y][systemBlockPos.x] == 1)
						{
							//マップのデータ削除
							gNowMap[systemBlockPos.y][systemBlockPos.x] = Obj_Id::NONE;
							((CBlock*)base[i].get())->IsCrash = true;
							isBreak = true;
							break;
						}
					}
				}
			}
		}
		if (isBreak) break;
	}

	if (IsEnd) return;

	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return;

	Point startPos = { pos.x + ImgWidth / 2 ,pos.y + ImgHeight / 2 };

	for (int i = 0; i < 4; i++)
	{
		Point expPos = { std::get<1>(ExplosionPointData[i]).x * std::get<2>(ExplosionPointData[i]) * 64,
						 std::get<1>(ExplosionPointData[i]).y * std::get<2>(ExplosionPointData[i]) * 64
		};

		//当たり判定
		if (p->m_pos.x < startPos.x + expPos.x && p->m_pos.x + 64 > startPos.x + expPos.x &&
			p->m_pos.y - WINDOW_HEADER < startPos.y + expPos.y - WINDOW_HEADER && p->m_pos.y - WINDOW_HEADER + 64 > startPos.y + expPos.y - WINDOW_HEADER)
		{
			p->SetPlayerDead(CPlayer::PlayerStateId::DEADplayer);
			IsEnd = true;
		}
	}

}

//爆発アニメーション処理
bool CExplosion::ExplosionAnim(int animMax, int* index)
{
	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < EXPLOSION_ANIM_FRAME)
	{
		AnimCnt++;//インクリメント
		return true;
	}
	//初期化
	AnimCnt = 0;

	//アニメーションの最大値以上の場合は初期化
	if (*index >= animMax - 1)
		return false;

	*index += 1;

	return true;
}

//爆弾描画(レベル別)
//ずれ分の座標x、y、描画個数、縦or横
std::tuple<Point,Point, int> CExplosion::DrawExplosion(float addPosX, float addPosY, int num, ExplosionEffectId dir)
{
	Point movePos{ addPosX * CHIP_SIZE, addPosY * CHIP_SIZE };

	Point drawPos{ pos.x + movePos.x, pos.y + movePos.y };

	MapPoint systemPos{ BombPos.x, BombPos.y };

	ExplosionDir expDir{ ExplosionDir::NONE_exp };

	int cnt = 0;
	std::tuple<Point, Point, int> data = std::make_tuple(Point{ 0,0 }, Point{ addPosX, addPosY }, 0);


	//爆発開始位置からのずれを設定
	if (dir == ExplosionEffectId::VERTICAL){
		if (addPosY == -1) {	//上
			std::get<0>(data).x = CHIP_SIZE / 2;
			std::get<0>(data).y = 0;
			expDir = ExplosionDir::UP_exp;
		}
		else{					//下
			std::get<0>(data).x = CHIP_SIZE / 2;
			std::get<0>(data).y = CHIP_SIZE;
			expDir = ExplosionDir::DOWN_exp;
		}
	}
	else {
		if (addPosX == -1) {	//左
			std::get<0>(data).x = 0;
			std::get<0>(data).y = CHIP_SIZE / 2;
			expDir = ExplosionDir::LEFT_exp;
		}
		else {					//右
			std::get<0>(data).x = CHIP_SIZE;
			std::get<0>(data).y = CHIP_SIZE / 2;
			expDir = ExplosionDir::RIGHT_exp;
		}
	}
	
	for (int i = 0; i < num; i++)
	{
		systemPos = {	systemPos.x + static_cast<int>(addPosX),
						systemPos.y + static_cast<int>(addPosY)
		};
		//先の升目がブロックの場合は描画終了
		if (gNowMap[systemPos.y][systemPos.x] == 0)
		{
			std::get<2>(data) = cnt;
			return data;
		}
		if (gNowMap[systemPos.y][systemPos.x] == 1 ||
			(CrashBlockPos[expDir].x == systemPos.x && CrashBlockPos[expDir].y == systemPos.y))
		{
			std::get<2>(data) = cnt + 1;
			CrashBlockPos[static_cast<int>(expDir)] = systemPos; //破壊可能ブロックの座標保存
			return data;
		}
		

		//爆発描画
		DrawExtendGraph(drawPos.x, drawPos.y, drawPos.x + ImgWidth, drawPos.y + ImgHeight,
			ExplosionImgHandle[EXPLOSION_ANIM_ORDER[AnimIndex] + static_cast<int>(dir)], true);

		//描画座標更新
		drawPos.x += movePos.x;
		drawPos.y += movePos.y;
		//システム上の座標更新
		systemPos.x += static_cast<int>(addPosX);
		systemPos.y += static_cast<int>(addPosY);
		cnt++;
	}

	std::get<2>(data) = cnt;
	return data;
}