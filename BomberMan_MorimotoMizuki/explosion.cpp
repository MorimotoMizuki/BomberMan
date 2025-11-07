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

	//ブロックオブジェクトと判定
	if (std::get<0>(ExplosionPointData[0]).x != 0)
	{
		for (int i = 0; i < base.size(); i++)
		{
			if (base[i]->ID == BLOCK)
			{
				if (((CBlock*)base[i].get())->tipNo == 1)
				{
					Point blockPos = base[i].get()->pos;
					MapPoint systemBlockPos = ((CBlock*)base[i].get())->SystemPos;

					DrawFormatString(0, 50, GetColor(255, 0, 0), "%.f\n%.f", blockPos.x, blockPos.y - WINDOW_HEADER);
					DrawFormatString(100, 50, GetColor(255, 0, 0), "%.f\n%.f", pos.x, pos.y - WINDOW_HEADER);
					for (int i = 0; i < 4; i++)
					{
						Point startPos = { pos.x + std::get<0>(ExplosionPointData[i]).x ,pos.y + std::get<0>(ExplosionPointData[i]).y };
						Point expPos = { std::get<1>(ExplosionPointData[i]).x * std::get<2>(ExplosionPointData[i]) * 32,
										 std::get<1>(ExplosionPointData[i]).y * std::get<2>(ExplosionPointData[i]) * 32 };


						if (blockPos.x < startPos.x + expPos.x && blockPos.x + 64 > startPos.x + expPos.x &&
							blockPos.y - WINDOW_HEADER < startPos.y + expPos.y - WINDOW_HEADER && blockPos.y - WINDOW_HEADER + 64 > startPos.y + expPos.y - WINDOW_HEADER)
						{
							//マップのデータ削除
							gNowMap[systemBlockPos.y][systemBlockPos.x] = Obj_Id::NONE;
							base[i].get()->FLAG = false;
						}
					}
				}
			}
		}
	}

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

}

CExplosion::~CExplosion()
{

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

	int cnt = 0;
	std::tuple<Point, Point, int> data = std::make_tuple(Point{ 0,0 }, Point{ addPosX, addPosY }, 0);


	//爆発開始位置からのずれを設定
	if (dir == ExplosionEffectId::VERTICAL){
		if (addPosY == -1) {	//上
			std::get<0>(data).x = CHIP_SIZE / 2;
			std::get<0>(data).y = 0;
		}
		else{					//下
			std::get<0>(data).x = CHIP_SIZE / 2;
			std::get<0>(data).y = CHIP_SIZE;
		}
	}
	else {
		if (addPosX == -1) {	//左
			std::get<0>(data).x = 0;
			std::get<0>(data).y = CHIP_SIZE / 2;
		}
		else {					//右
			std::get<0>(data).x = CHIP_SIZE;
			std::get<0>(data).y = CHIP_SIZE / 2;
		}
	}
	
	for (int i = 0; i < num; i++)
	{
		systemPos = {	systemPos.x + static_cast<int>(addPosX),
						systemPos.y + static_cast<int>(addPosY)
		};
		//先の升目がブロックの場合は描画終了
		if (gNowMap[systemPos.y][systemPos.x] == Obj_Id::BLOCK)
		{
			std::get<2>(data) = cnt;
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