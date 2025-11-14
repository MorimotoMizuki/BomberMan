#pragma once

#include"objBase.h"

class CBlock : public BaseVector
{
public:

	//クラッシュブロックのアニメーションフレーム数
	static constexpr auto CRASH_ANIM_FRAME = 18;

	//クラッシュアニメーションの画像の数
	static constexpr auto CRASH_IMG_NUM = 2;

public:

	//座標、システム上の座標、ID、画像ハンドル
	CBlock(Point, MapPoint, int, int);
	~CBlock();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//クラッシュアニメーション
	void CrashBlockAnim();
public:

	//マップチップNo
	int tipNo{ -1 };

	//システム上(マップ上)の座標
	MapPoint SystemPos{ 0,0 };

	//クラッシュフラグ
	bool IsCrash{ false };

private:

	//画面スクロールによる座標のずれ
	Point DisplacementPos{ 0,0 };

	//プレイヤーの画像ハンドル配列
	int CrashAnimImgHandle[CRASH_IMG_NUM];
	//アニメーションの画像インデックス
	int AnimIndex{ 0 };
	//アニメーションカウント
	int AnimCnt{ CRASH_ANIM_FRAME };

};