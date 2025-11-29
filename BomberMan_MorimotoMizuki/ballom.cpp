#include"obj.h"
#include"function.h"

CBallom::CBallom(Point p ,int _img)
{
	img = _img;

	pos = p;

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	ID = Obj_Id::ENEMY;
	pri = Pri_Id::pENEMY;
}

int CBallom::Action(vector<unique_ptr<BaseVector>>& base)
{
	return 0;
}

void CBallom::Draw()
{
	//‰æ‘œ•`‰æ
	DrawRectGraph(pos.x, pos.y, CutX, CutY, ImgWidth, ImgHeight, img, true);

}

CBallom::~CBallom()
{

}