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

	//バロムの移動速度
	static constexpr auto BALLOM_SPEED = 2.0f;

	//左右上下
	static constexpr MapPoint ADD_VEC[4] = {
		{-1,0},
		{1,0},
		{0,-1},
		{0,1}
	};

	enum BallomDir
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};

public:

	//画像ハンドル、システム上の座標
	CBallom(Point, MapPoint);
	~CBallom();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//バロムの移動処理
	void BallomMove(vector<unique_ptr<BaseVector>>&);

	//バロムの移動方向設定処理
	void SetBallomMoveDir(vector<unique_ptr<BaseVector>>&);

	//バロムアニメーション処理
	void BallomAnim(int animMax, int* index, bool loop);

	//死亡処理
	void EnemyDead();

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

	//死亡フラグ
	bool IsDead{ false };

	//死亡カウント
	int DeadCnt{ 0 };
};