#pragma once

#include"objBase.h"
#include<map>
#include<functional>

class CItem : public BaseVector
{
public:

	//アイテムの画像の数
	static constexpr auto ITEM_IMG_NUM = 8;

public:

	//座標、システム上の座標、アイテムのID
	CItem(Point, MapPoint, Item_Id);
	~CItem();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//アイテム削除
	void DeleteItem();

	//各アイテムの関数設定
	void SetItemFunction();

	void FirePowerAction();			//火力アップ					: オニール
	void BombAction();				//爆弾の置ける数アップ			: バロム
	void RemoteControllerAction();	//爆弾がBボタンで自由に爆発可能	: コンドリア
	void BootsAction();				//移動速度アップ				: ダル
	void BombPassingAction();		//爆弾の上を歩けるようになる	: オバピー
	void WallPassingAction();		//壁の上を歩けるようになる		: ミンボー
	void FlameBarrierAction();		//爆風で死ななくなる			: バース
	void PerfectManAction();		//30秒間無敵になる				: ポンタン

public:

	//アイテムのID
	Item_Id ItemID;

private:

	//プレイヤーとの差分
	float Distance{ 0.0f };

	//アイテムの画像ハンドル
	int ItemImgHandle[ITEM_IMG_NUM];

	//アイテムごとの獲得時処理用関数マップ
	std::map<Item_Id, std::function<void()>> ItemFunctions;

	//SE　アイテム獲得サウンド用
	int SE_ItemGet{ -1 };
};