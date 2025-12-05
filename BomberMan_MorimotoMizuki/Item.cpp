#include"obj.h"
#include"function.h"

CItem::CItem(Point p, MapPoint system_p, Item_Id item_id)
{
	LoadDivGraph("image\\Item.png", ITEM_IMG_NUM, 4, 2, IMGSIZE16, IMGSIZE16, ItemImgHandle);

	pos = p;

	SystemPos = system_p;

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	ItemID = item_id;

	ID  = Obj_Id::ITEM;
	pri = Pri_Id::pITEM;
}

int CItem::Action(vector<unique_ptr<BaseVector>>& base)
{
	//ƒvƒŒƒCƒ„[‚ðŽæ“¾
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p != nullptr)
		Distance = p->Distance;

	return 0;
}

void CItem::Draw()
{
	//‰æ‘œ•`‰æ
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, img, true);

	//DrawFormatString(WINDOW_WIDTH / 2 - 200, 50, GetColor(255, 255, 255), "%f\n%f", pos.x, pos.y - WINDOW_HEADER);
	DrawFormatString(WINDOW_WIDTH / 2 - 200, 50, GetColor(255, 255, 255), "%d\n%d", SystemPos.x, SystemPos.y);
}

CItem::~CItem()
{
	for (int i = 0; i < ITEM_IMG_NUM; i++)
		DeleteGraph(ItemImgHandle[i]);
}