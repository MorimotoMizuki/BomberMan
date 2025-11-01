#include"obj.h"
#include"function.h"

CBlock::CBlock(Point p, int No, int _img)
{
	img = _img;

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	pos = p;

	//切り取り位置 : 16 -> 画像の横チップ数
	CutX = (No % 16) * ImgWidth;
	CutY = (No / 16) * ImgHeight;

	tipNo = No;

	ID = Obj_Id::BLOCK;
	pri = Pri_Id::pBLOCK;
}

int CBlock::Action(vector<unique_ptr<BaseVector>>& base)
{

	return 0;
}

void CBlock::Draw()
{
	//画像描画
	DrawRectGraph(
		pos.x, pos.y,
		CutX, CutY,
		ImgWidth, ImgHeight,
		img,
		false	//不透過
	);

	//DrawFormatString(pos.x, pos.y, GetColor(255, 0, 0), "%.2f\n%.2f", pos.x, pos.y);
}

CBlock::~CBlock()
{

}