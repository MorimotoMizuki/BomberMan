#include"obj.h"
#include"function.h"
#include"map.h"

CExplosion::CExplosion(Point p, int bombLevel)
{
	LoadDivGraph("image\\explosion.png", EXPLOSION_IMG_NUM, 4, 3, IMGSIZE32, IMGSIZE32, ExplosionImgHandle);

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	pos = p;

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
	DrawExplosion(0, -ImgHeight	, BombLevel, ExplosionEffectId::VERTICAL);
	DrawExplosion(0,  ImgHeight	, BombLevel, ExplosionEffectId::VERTICAL);
	DrawExplosion(-ImgWidth, 0, BombLevel, ExplosionEffectId::HORIZONTAL);
	DrawExplosion( ImgWidth, 0, BombLevel, ExplosionEffectId::HORIZONTAL);
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
void CExplosion::DrawExplosion(float addPosX, float addPosY, int num, ExplosionEffectId dir)
{
	Point movePos{ addPosX, addPosY };

	Point drawPos{ pos.x + addPosX, pos.y + addPosY };

	for (int i = 0; i < num; i++)
	{
		//ここで隣がブロックの場合は終了の処理を書く ,また　クラッシュブロックだったら壊す

		 
		//爆発描画
		DrawExtendGraph(drawPos.x, drawPos.y, drawPos.x + ImgWidth, drawPos.y + ImgHeight,
			ExplosionImgHandle[EXPLOSION_ANIM_ORDER[AnimIndex] + static_cast<int>(dir)], true);

		drawPos.x += movePos.x;
		drawPos.y += movePos.y;
	}
}
