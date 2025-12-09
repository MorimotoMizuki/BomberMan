#include"obj.h"
#include"function.h"

CBlock::CBlock(Point p, MapPoint system_p, int No, int _img, Item_Id item_id)
{
	img = _img;

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	pos = p;

	//切り取り位置 : 16 -> 画像の横チップ数
	CutX = (No % 16) * ImgWidth;
	CutY = (No / 16) * ImgHeight;

	tipNo = No;

	if (tipNo == 1)
		LoadDivGraph("image\\block_anim.png", CRASH_IMG_NUM, 2, 1, IMGSIZE64, IMGSIZE64, CrashAnimImgHandle);

	ItemId = item_id; //アイテムID登録

	SystemPos = system_p;

	ID  = Obj_Id::BLOCK;
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

	//DrawFormatString(pos.x - Distance, pos.y, GetColor(0, 0, 0), "%d", IsCrash);

	if (ItemId != Item_Id::None)
		DrawFormatString(WINDOW_WIDTH / 2 - 200, 50, GetColor(255, 255, 255), "%d\n%d", SystemPos.x, SystemPos.y);

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

	//アイテムがあった場合はすぐに消す
	if (ItemId != Item_Id::None)
		FLAG = false;

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