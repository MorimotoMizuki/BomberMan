#include"obj.h"
#include"function.h"
#include"map.h"

CBomb::CBomb(Point p, int diedFrame)
{
	LoadDivGraph("image\\bomb.png", BOMB_IMG_NUM, 3, 1, IMGSIZE16, IMGSIZE16, BombImgHandle);

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	int mapX = p.x / IMGSIZE64;
	int mapY = p.y / IMGSIZE64;

	pos.x = mapX * CHIP_SIZE;
	pos.y = mapY * CHIP_SIZE + (WINDOW_HEADER - IMGSIZE64);

	DiedFrame = diedFrame;

	ID = Obj_Id::BOMB;
	pri = Pri_Id::pBOMB;

	//設置している爆弾の数 : 増加
	gNowBombNum++;
}

int CBomb::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);

	//プレイヤーの座標がスクロールする座標になった場合
	if (p->pos.x == DRAW_CHIP_W * CHIP_SIZE / 2)
	{
		//プレイヤーの移動と逆方向に移動
		pos.x -= p->vec.x;
	}

	SurvivalFrame++;
	if (SurvivalFrame >= DiedFrame)
	{
		ExplosionEffect(base);
		FLAG = false;
	}

	//爆弾アニメーション
	BombAnim(BOMB_ANIM_NUM, &AnimIndex);

	return 0;
}

void CBomb::Draw()
{
	//画像描画
	DrawExtendGraph(pos.x, pos.y, pos.x + ImgWidth, pos.y + ImgHeight, BombImgHandle[BOMB_ANIM_ORDER[AnimIndex]], true);

	//デバッグ
	DrawFormatString(WINDOW_WIDTH / 2, 50, GetColor(255, 255, 255), "%f\n%f", pos.x, pos.y);
}

//爆弾アニメーション処理
void CBomb::BombAnim(int animMax, int* index)
{
	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < BOMB_ANIM_FRAME)
	{
		AnimCnt++;//インクリメント
		return;
	}
	//初期化
	AnimCnt = 0;

	//アニメーションの最大値以上の場合は初期化
	if (*index >= animMax - 1)
		*index = 0;
	else
		*index += 1;

}


CBomb::~CBomb()
{
	//現在設置している爆弾の数 : 減少
	gNowBombNum--;
}

//爆発エフェクト
void CBomb::ExplosionEffect(vector<unique_ptr<BaseVector>>& base)
{
	//爆発エフェクト生成 
	base.emplace_back((unique_ptr<BaseVector>)new CExplosion(pos, gPlayerStatus.bombLevel));
}