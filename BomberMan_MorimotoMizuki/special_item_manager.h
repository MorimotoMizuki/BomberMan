#pragma once

#include"objBase.h"

class CSpecial_Item_Manager : public BaseVector
{
public:

	CSpecial_Item_Manager(Special_Item_Id special_item_id);
	~CSpecial_Item_Manager();

	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//各アイテムの関数設定
	void SetItemCreateFunction();

	//各アイテムの処理関数
	bool B_PanelCreateAction();
	bool GoddessCreateAction();
	bool ColaCreateAction();
	bool FamicomCreateAction();
	bool Mr_NakamotoCreateAction();
	bool DezenimanCreateAction();


private:

	Special_Item_Id Special_Item_ID;

	//アイテムごとの獲得時処理用関数マップ
	std::map<Special_Item_Id, std::function<bool()>> ItemCreateFunctions;
};