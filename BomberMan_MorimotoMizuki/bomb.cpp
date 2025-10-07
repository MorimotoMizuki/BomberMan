#include"obj.h"
#include"function.h"

CBomb::CBomb(Point p, int diedFrame)
{
	LoadDivGraph("image\\bomb.png", BOMB_IMG_NUM, 3, 1, IMGSIZE16, IMGSIZE16, BombImgHandle);

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	pos = p;
	DiedFrame = diedFrame;

	ID = Obj_Id::BOMB;
	pri = Pri_Id::pBOMB;

	//設置している爆弾の数 : 増加
	gNowBombNum++;
}

int CBomb::Action(vector<unique_ptr<BaseVector>>& base)
{
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
	base.emplace_back((unique_ptr<BaseVector>)new CExplosion(pos, ExplosionEffectId::eCENTER));

	//爆弾レベル別で生成するエフェクトの変更
	switch (gPlayerStatus.bombLevel)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}

}