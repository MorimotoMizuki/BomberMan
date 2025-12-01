#pragma once

#include"objBase.h"

class CBallom : public BaseVector
{
public:

	//バロムのアニメーションフレーム数
	static constexpr auto BALLOM_ANIM_FRAME = 8;

	//バロムの画像の数
	static constexpr auto BALLOM_IMG_NUM = 4;

	//バロムのアニメーション数
	static constexpr auto BALLOM_ANIM_NUM = 4;

	//バロムアニメーションの順番
	static constexpr int BALLOM_ANIM_ORDER[BALLOM_ANIM_NUM] = { 0,1,2,1 };

	//バロムのアニメーションフレーム数
	static constexpr auto BALLOM_SPEED = 4.0f;

public:

	//画像ハンドル、システム上の座標
	CBallom(Point, MapPoint);
	~CBallom();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//バロムの移動処理
	void BallomMove();

	//バロムアニメーション処理
	void BallomAnim(int animMax, int* index);

public:

	MapPoint SystemPos{ 0,0 };

private:

	//画像ハンドル
	int BallomImgHandle[BALLOM_IMG_NUM]{ 0,0,0,0 };

	//アニメーションの画像インデックス
	int AnimIndex{ 0 };
	//アニメーションカウント
	int AnimCnt{ BALLOM_ANIM_FRAME };

	//プレイヤーとの差分
	float Distance{ 0.0f };

};