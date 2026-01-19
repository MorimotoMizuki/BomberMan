#include"obj.h"
#include"function.h"

CSpecial_Item::CSpecial_Item(Point p, MapPoint system_p, Special_Item_Id s_item_id)
{
	CBaseItem::Constructor(p, system_p); //ベースのコンストラクタ

	LoadDivGraph("image\\hidden_character.png", ITEM_IMG_NUM, 6, 1, IMGSIZE16, IMGSIZE16, ItemImgHandle);

	SetItemFunction(); //各アイテムの関数設定

	Special_ItemID = s_item_id;

	Score = 0;
}

int CSpecial_Item::Action(vector<unique_ptr<BaseVector>>& base)
{
	CBaseItem::ItemAction(base); //ベースの更新処理

	return 0;
}

void CSpecial_Item::Draw()
{
	if (!draw_flag) return;

	//画像描画
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, ItemImgHandle[Special_ItemID], true);
}

CSpecial_Item::~CSpecial_Item()
{
	CBaseItem::Destructor(); //ベースのデストラクタ

	for (int i = 0; i < ITEM_IMG_NUM; i++)
		DeleteGraph(ItemImgHandle[i]);
}

//アイテム獲得時処理
void CSpecial_Item::ItemGetAction()
{
	if (ItemFunctions.contains(Special_ItemID))
		ItemFunctions[Special_ItemID]();   // 対応する関数を呼び出す
}

//アイテム爆破時処理
void CSpecial_Item::ItemExplosionAction(vector<unique_ptr<BaseVector>>& base)
{

}

//各アイテムの関数設定
void CSpecial_Item::SetItemFunction()
{
	ItemFunctions[Special_Item_Id::B_Panel]		= [&]() {B_PanelAction(); };
	ItemFunctions[Special_Item_Id::Goddess]		= [&]() {GoddessAction(); };
	ItemFunctions[Special_Item_Id::Cola]		= [&]() {ColaAction(); };
	ItemFunctions[Special_Item_Id::Famicom]		= [&]() {FamicomAction(); };
	ItemFunctions[Special_Item_Id::Mr_Nakamoto] = [&]() {Mr_NakamotoAction(); };
	ItemFunctions[Special_Item_Id::Dezeniman]	= [&]() {DezenimanAction(); };
}

//Bパネル 10000点
void CSpecial_Item::B_PanelAction()
{
	Score = 10000;

	CBaseItem::DeleteItem();
}
//ゴーデス 20000点
void CSpecial_Item::GoddessAction()
{
	Score = 20000;

	CBaseItem::DeleteItem();
}
//コーラ 30000点
void CSpecial_Item::ColaAction()
{
	Score = 30000;

	CBaseItem::DeleteItem();
}
//ファミコン 500000点
void CSpecial_Item::FamicomAction() 
{
	Score = 500000;

	CBaseItem::DeleteItem();
}
//中本さん 10000000点
void CSpecial_Item::Mr_NakamotoAction()
{
	Score = 10000000;

	CBaseItem::DeleteItem();
}
//デゼニマン 20000000点 
void CSpecial_Item::DezenimanAction()
{
	Score = 20000000;

	CBaseItem::DeleteItem();
}