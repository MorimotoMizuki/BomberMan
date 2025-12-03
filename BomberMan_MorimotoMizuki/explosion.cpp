#include"obj.h"
#include"function.h"
#include"map.h"

CExplosion::CExplosion(Point p, MapPoint bombP, int bombLevel)
{
	LoadDivGraph("image\\explosion.png", EXPLOSION_IMG_NUM, 4, 3, IMGSIZE32, IMGSIZE32, ExplosionImgHandle);

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	//爆弾の座標
	BombPos = bombP;

	//座標を升目 * チップサイズ - 画面上のずれ　で計算
	pos.x = BombPos.x * CHIP_SIZE;
	pos.y = BombPos.y * CHIP_SIZE + WINDOW_HEADER;

	//爆弾レベル
	BombLevel = bombLevel;

	ID = Obj_Id::EXPLOSION;
	pri = Pri_Id::pEXPLOSION;
}

int CExplosion::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if(p != nullptr)
		Distance = p->Distance;

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
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, ExplosionImgHandle[EXPLOSION_ANIM_ORDER[AnimIndex]], true);

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
	for (int i = 0; i < EXPLOSION_IMG_NUM; i++)
		DeleteGraph(ExplosionImgHandle[i]);
}

//当たり判定
void CExplosion::HitAction(vector<unique_ptr<BaseVector>>& base)
{
	if (std::get<0>(ExplosionPointData[0]).x == 0)
		return;

	//アニメーションインデックスが一定以上の場合は終了
	if (AnimIndex >= EXPLOSION_ANIM_NUM - 1 - 2)
		return; 

	bool isBreak{ false };	
	for (int i = 0; i < base.size(); i++)
	{
		//削除対象のオブジェクトはスキップ
		if (!base[i]->FLAG || !base[i]->draw_flag)
			continue;

		//ブロックオブジェクトと判定
		if (base[i]->ID == BLOCK)
		{
			if (((CBlock*)base[i].get())->tipNo == 1)
			{
				Point blockPos = base[i].get()->pos;
				MapPoint systemBlockPos = ((CBlock*)base[i].get())->SystemPos;

				for (int j = 0; j < 4; j++)
				{
					Point startPos = { pos.x + std::get<0>(ExplosionPointData[j]).x ,pos.y + std::get<0>(ExplosionPointData[j]).y };
					Point expPos = { std::get<1>(ExplosionPointData[j]).x * std::get<2>(ExplosionPointData[j]) * 32,
									 std::get<1>(ExplosionPointData[j]).y * std::get<2>(ExplosionPointData[j]) * 32
					};

					//当たり判定
					if (blockPos.x < startPos.x + expPos.x && blockPos.x + 64 > startPos.x + expPos.x &&
						blockPos.y - WINDOW_HEADER < startPos.y + expPos.y - WINDOW_HEADER && blockPos.y - WINDOW_HEADER + 64 > startPos.y + expPos.y - WINDOW_HEADER)
					{
						if (gNowMap[systemBlockPos.y][systemBlockPos.x] == 1)
						{
							//マップのデータ削除
							gNowMap[systemBlockPos.y][systemBlockPos.x] = Obj_Id::NONE;
							isBreak = true;
							break;
						}
					}
				}
			}
		}
		if (isBreak) break;
	}

	Point startPos = { pos.x + ImgWidth / 2 ,pos.y + ImgHeight / 2 };

	for (int i = 0; i < base.size(); i++)
	{
		//削除対象のオブジェクトはスキップ
		if (!base[i]->FLAG || !base[i]->draw_flag)
			continue;

		Point enemyPos = base[i].get()->pos;
		//敵との判定
		if (base[i]->ID == ENEMY)
		{
			for (int j = 0; j < 4; j++)
			{
				Point expPos = { std::get<1>(ExplosionPointData[j]).x * std::get<2>(ExplosionPointData[j]) * 64,
								 std::get<1>(ExplosionPointData[j]).y * std::get<2>(ExplosionPointData[j]) * 64
				};
				//当たり判定
				if (enemyPos.x < startPos.x + expPos.x && enemyPos.x + 64 > startPos.x + expPos.x &&
					enemyPos.y - WINDOW_HEADER < startPos.y + expPos.y - WINDOW_HEADER && enemyPos.y - WINDOW_HEADER + 64 > startPos.y + expPos.y - WINDOW_HEADER)
				{
					((CBallom*)base[i].get())->EnemyDead();
				}
			}
		}
	}

	if (IsEnd) return;

	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return;


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

	ExplosionDir expDir{ ExplosionDir::UP_exp };

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
			CrashBlockPos[expDir].x = systemPos.x; //破壊可能ブロックの座標保存
			CrashBlockPos[expDir].y = systemPos.y; //破壊可能ブロックの座標保存
			return data;
		}	

		//爆発描画
		DrawExtendGraph(drawPos.x - Distance, drawPos.y, drawPos.x + ImgWidth - Distance, drawPos.y + ImgHeight,
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