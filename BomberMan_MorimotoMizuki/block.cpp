#include"obj.h"
#include"function.h"

CBlock::CBlock(Point p, int id)
{
	img = LoadGraph("image\\normal_block.png");

	pos = p;
	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;


	ID = id;
	pri = Pri_Id::pBLOCK;
}

int CBlock::Action(vector<unique_ptr<BaseVector>>& base)
{

	return 0;
}

void CBlock::Draw()
{
	//‰æ‘œ•`‰æ
	DrawGraph(pos.x, pos.y, img, true);
}

CBlock::~CBlock()
{

}