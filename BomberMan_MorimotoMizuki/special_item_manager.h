#pragma once

#include"objBase.h"
#include<vector>
#include<unordered_set>

class CSpecial_Item_Manager : public BaseVector
{
private:

	//マップの座標用 : ゴーデスの外周処理に使う
	static constexpr auto left		= 1;
	static constexpr auto right		= MAP_CHIP_W - 2;
	static constexpr auto top		= 1;
	static constexpr auto bottom	= MAP_CHIP_H - 2;

public:

	CSpecial_Item_Manager(Special_Item_Id special_item_id);
	~CSpecial_Item_Manager();

	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//各アイテムの関数設定
	void SetItemCreateFunction();

	//各アイテムの処理関数
	bool B_PanelCreateAction(vector<unique_ptr<BaseVector>>&);
	bool GoddessCreateAction(vector<unique_ptr<BaseVector>>&);
	bool ColaCreateAction(vector<unique_ptr<BaseVector>>&);
	bool FamicomCreateAction(vector<unique_ptr<BaseVector>>&);
	bool Mr_NakamotoCreateAction(vector<unique_ptr<BaseVector>>&);
	bool DezenimanCreateAction(vector<unique_ptr<BaseVector>>&);

private:

	Special_Item_Id Special_Item_ID;

	//アイテムごとの獲得時処理用関数マップ
	std::map<Special_Item_Id, std::function<bool(vector<unique_ptr<BaseVector>>& base)>> ItemCreateFunctions;

	//各アイテムの出現フラグ
	bool IsPopItem[SPECIAL_ITEM_VARIATION]{ false,false,false,false,false,false };

	//各アイテム出現条件用の変数↓

	//Bパネル
	bool IsPlayer_match_Goal{ false }; //プレイヤーとゴールの座標一致フラグ
	
	//ゴーデス
	std::vector<MapPoint> OuterRoute; //外周配列
	std::unordered_set<int> PassedOuterTiles; //踏破済みタイル配列

	//コーラ
	Move_Id PrevMove_Dir{ Move_Id::NONE_KEY }; //プレイヤーの移動方向保存用
	int MoveCnt{ 0 }; //移動フレームカウント

	//ファミコン

	//中本さん

	//デゼニマン
	bool IsAllExplosionCrashBlock{ false }; //破壊可能ブロックを全て爆破フラグ
};