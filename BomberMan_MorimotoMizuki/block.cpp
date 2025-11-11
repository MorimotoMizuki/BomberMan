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

	SystemPos = system_p;

	ID = Obj_Id::BLOCK;
	pri = Pri_Id::pBLOCK;
}

int CBlock::aaa{ 0 };

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

	//DrawFormatString(pos.x, pos.y, GetColor(0, 0, 0), "%d\n%d", SystemPos.x * 64, SystemPos.y * 64);
	DrawFormatString(pos.x, pos.y, GetColor(0, 0, 0), "%d\n%d", SystemPos.x, SystemPos.y);

}

CBlock::~CBlock()
{

}