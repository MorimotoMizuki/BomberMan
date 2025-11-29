#include"obj.h"
#include"function.h"
#include"map.h"

CBomb::CBomb(MapPoint mapPos, int diedFrame)
{
	LoadDivGraph("image\\bomb.png", BOMB_IMG_NUM, 3, 1, IMGSIZE16, IMGSIZE16, BombImgHandle);

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	//システム上の座標を設定
	SystemMap = mapPos;

	//座標を升目 * チップサイズ - 画面上のずれ　で計算
	pos.x = SystemMap.x * CHIP_SIZE;
	pos.y = SystemMap.y * CHIP_SIZE + WINDOW_HEADER;

	//爆弾の削除フレーム数設定
	DiedFrame = diedFrame;

	ID = Obj_Id::BOMB;
	pri = Pri_Id::pBOMB;

	//現在のマップに爆弾を配置
	gNowMap[SystemMap.y][SystemMap.x] = ID;

	//設置している爆弾の数 : 増加
	gNowBombNum++;
}

int CBomb::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	Distance = p->Distance;

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
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, BombImgHandle[BOMB_ANIM_ORDER[AnimIndex]], true);

	//DrawFormatString(pos.x + 30, pos.y, GetColor(255, 0, 0), "%.f\n%.f", pos.x, pos.y);
	//DrawFormatString(pos.x + 30, pos.y, GetColor(255, 0, 0), "%d\n%d", SystemMap.x, SystemMap.y);

	//DrawBox(pos.x, pos.y, pos.x + ImgWidth, pos.y + ImgHeight, GetColor(255, 0, 0), false);
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
	//システム上のマップから爆弾を削除
	gNowMap[SystemMap.y][SystemMap.x] = Obj_Id::NONE;

	//爆発エフェクト生成 
	base.emplace_back((unique_ptr<BaseVector>)new CExplosion(pos, SystemMap, gPlayerStatus.bombLevel));
}