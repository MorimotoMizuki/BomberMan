#pragma once

#include"objBase.h"

class CBlock : public BaseVector
{
public:

public:

	//座標、ID、画像ハンドル
	CBlock(Point, int, int);
	~CBlock();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

public:

	//マップチップNo
	int tipNo{ -1 };

private:

	//画面スクロールによる座標のずれ
	Point DisplacementPos{ 0,0 };
};