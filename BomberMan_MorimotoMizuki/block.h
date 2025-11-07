#pragma once

#include"objBase.h"

class CBlock : public BaseVector
{
public:

public:

	//座標、システム上の座標、ID、画像ハンドル
	CBlock(Point, MapPoint, int, int);
	~CBlock();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

public:

	//マップチップNo
	int tipNo{ -1 };

	//システム上(マップ上)の座標
	MapPoint SystemPos{ 0,0 };

private:

	//画面スクロールによる座標のずれ
	Point DisplacementPos{ 0,0 };

};