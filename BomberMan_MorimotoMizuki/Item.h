#pragma once

#include"objBase.h"

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

public:

	//アイテムのID
	Item_Id ItemID;

private:

	//プレイヤーとの差分
	float Distance{ 0.0f };

	//アイテムの画像ハンドル
	int ItemImgHandle[ITEM_IMG_NUM];
};