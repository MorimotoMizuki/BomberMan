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
		LoadDivGraph("image\\block_anim.png", CRASH_IMG_NUM, 2, 1, IMGSIZE64, IMGSIZE64, CrashAnimImgHandle);

	SystemPos = system_p;

	ID = Obj_Id::BLOCK;
	pri = Pri_Id::pBLOCK;
}

int CBlock::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if(p != nullptr)
		Distance = p->Distance;

	if (!IsCrash)
	{
		if (gNowMap[SystemPos.y][SystemPos.x] != tipNo)
		{
			IsCrash = true;
		}
	}

	//クラッシュアニメーション
	CrashBlockAnim();

	return 0;
}

void CBlock::Draw()
{
	if (!draw_flag) return;

	//画像描画
	if (IsCrash)
	{
		DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, CrashAnimImgHandle[AnimIndex], true);
	}
	else
	{
		DrawRectGraph(pos.x - Distance, pos.y, CutX, CutY, ImgWidth, ImgHeight, img, false);
	}

	//DrawFormatString(pos.x, pos.y, GetColor(0, 0, 0), "%d\n%d", SystemPos.x * 64, SystemPos.y * 64);
	//DrawFormatString(pos.x - Distance, pos.y, GetColor(0, 0, 0), "%d", IsCrash);

}

CBlock::~CBlock()
{
	if (tipNo == 1) {
		for (int i = 0; i < CRASH_IMG_NUM; i++)
			DeleteGraph(CrashAnimImgHandle[i]);
	}
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

	if (AnimIndex >= CRASH_IMG_NUM - 1)
	{
		FLAG = false;
		return;
	}
	else
		AnimIndex += 1;
}