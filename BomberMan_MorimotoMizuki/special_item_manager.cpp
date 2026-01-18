#include"obj.h"
#include"function.h"

CSpecial_Item_Manager::CSpecial_Item_Manager(Special_Item_Id special_item_id)
{
	Special_Item_ID = special_item_id;

	SetItemCreateFunction(); //アイテムの生成条件関数の設定
}

int CSpecial_Item_Manager::Action(vector<unique_ptr<BaseVector>>& base)
{
	if (ItemCreateFunctions.contains(Special_Item_ID)) {

		//アイテムの生成条件を満たしていた場合
		if (ItemCreateFunctions[Special_Item_ID]())
		{
			//プレイヤーを取得
			CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);

			MapPoint s_p{ 0,0 };
			while (true)
			{
				s_p = { Range_Random_Number(1, MAP_CHIP_W), Range_Random_Number(1, MAP_CHIP_H) };

				//プレイヤーの座標と同じ座標の場合は再計算
				if (s_p.x == p->SystemPos.x && s_p.y == p->SystemPos.y)
					continue;

				//空白の場合は生成座標確定
				if (gNowMap[s_p.y][s_p.x] == Obj_Id::NONE) break;
			}

			Point c_p{ s_p.x * CHIP_SIZE, s_p.y * CHIP_SIZE + WINDOW_HEADER };

			//隠れアイテム生成
			base.emplace_back((unique_ptr<BaseVector>) new CSpecial_Item(c_p, s_p, Special_Item_ID));
		}
	}

	return 0;
}

void CSpecial_Item_Manager::Draw()
{

}

CSpecial_Item_Manager::~CSpecial_Item_Manager()
{

}

//各アイテムの生成関数設定
void CSpecial_Item_Manager::SetItemCreateFunction()
{
	ItemCreateFunctions[Special_Item_Id::B_Panel]		= [&]() -> bool { return B_PanelCreateAction(); };
	ItemCreateFunctions[Special_Item_Id::Goddess]		= [&]() -> bool { return GoddessCreateAction(); };
	ItemCreateFunctions[Special_Item_Id::Cola]			= [&]() -> bool { return ColaCreateAction(); };
	ItemCreateFunctions[Special_Item_Id::Famicom]		= [&]() -> bool { return FamicomCreateAction(); };
	ItemCreateFunctions[Special_Item_Id::Mr_Nakamoto]	= [&]() -> bool { return Mr_NakamotoCreateAction(); };
	ItemCreateFunctions[Special_Item_Id::Dezeniman]		= [&]() -> bool { return DezenimanCreateAction(); };
}

//各アイテムの生成条件関数
bool CSpecial_Item_Manager::B_PanelCreateAction()
{
	return false;
}
bool CSpecial_Item_Manager::GoddessCreateAction()
{
	return false;
}
bool CSpecial_Item_Manager::ColaCreateAction()
{
	return false;
}
bool CSpecial_Item_Manager::FamicomCreateAction()
{
	return false;
}
bool CSpecial_Item_Manager::Mr_NakamotoCreateAction()
{
	return false;
}
bool CSpecial_Item_Manager::DezenimanCreateAction()
{
	return false;
}