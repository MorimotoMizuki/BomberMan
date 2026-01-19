#pragma once

#include"objBase.h"
#include"base_item.h"
#include<map>
#include<functional>

class CSpecial_Item : public CBaseItem
{
private:

	//アイテムの画像の数
	static constexpr auto ITEM_IMG_NUM = SPECIAL_ITEM_VARIATION;

public:

	//コンストラクタ : 座標、システム上の座標、スペシャルアイテムID
	CSpecial_Item(Point p, MapPoint system_p, Special_Item_Id s_item_id);
	~CSpecial_Item();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//アイテム獲得時処理
	void ItemGetAction() override;

	//アイテム爆破時処理
	void ItemExplosionAction(vector<unique_ptr<BaseVector>>& base) override;

	//各アイテムの関数設定
	void SetItemFunction() override;

	//各アイテムの処理関数
	void B_PanelAction();
	void GoddessAction();
	void ColaAction();
	void FamicomAction();
	void Mr_NakamotoAction();
	void DezenimanAction();

public:

	//スペシャルアイテムID
	Special_Item_Id Special_ItemID;

private:

	//アイテムの画像ハンドル
	int ItemImgHandle[ITEM_IMG_NUM];

	//アイテムごとの獲得時処理用関数マップ
	std::map<Special_Item_Id, std::function<void()>> ItemFunctions;
};