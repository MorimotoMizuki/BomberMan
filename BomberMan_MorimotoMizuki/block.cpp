#include"obj.h"
#include"function.h"

CBlock::CBlock(Point p, MapPoint system_p, int No, int _img)
{
	img = _img;

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	pos = p;

	//切り取り位置 : 16 -> 画像の横チップ数
	CutX = (No % 16) * ImgWidth;
	CutY = (No / 16) * ImgHeight;

	tipNo = No;

	if (tipNo == 1)
		LoadDivGraph("image\\player.png", CRASH_IMG_NUM, 2, 1, IMGSIZE64, IMGSIZE64, CrashAnimImgHandle);

	SystemPos = system_p;

	ID = Obj_Id::BLOCK;
	pri = Pri_Id::pBLOCK;
}

int CBlock::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p->vec.x == 0.0f) {
		DisplacementPos.x = p->m_pos.x - p->pos.x;
	}
	if (p->vec.y == 0.0f) {
		DisplacementPos.y = p->m_pos.y - p->pos.y;
	}

	//クラッシュアニメーション
	CrashBlockAnim();

	return 0;
}

void CBlock::Draw()
{
	//画像描画
	if (IsCrash)
	{
		DrawRectGraph(pos.x, pos.y, CutX, CutY, ImgWidth, ImgHeight, CrashAnimImgHandle[AnimIndex], false);
	}
	else
	{
		DrawRectGraph(pos.x, pos.y, CutX, CutY, ImgWidth, ImgHeight, img, false);
	}

	//DrawFormatString(pos.x, pos.y, GetColor(0, 0, 0), "%d\n%d", SystemPos.x * 64, SystemPos.y * 64);
	DrawFormatString(pos.x, pos.y, GetColor(0, 0, 0), "%d", IsCrash);

}

CBlock::~CBlock()
{

}

void CBlock::CrashBlockAnim()
{
	if (tipNo != 1) return;

	if (!IsCrash) return;

	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < CRASH_ANIM_FRAME)
	{
		AnimCnt++;//インクリメント
		return;
	}

	//初期化
	AnimCnt = 0;

	if (AnimIndex >= CRASH_IMG_NUM)
	{
		//FLAG = false;
		return;
	}
	else
		AnimIndex += 1;
}