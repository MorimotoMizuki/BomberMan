#pragma once

#include"objBase.h"
#include"astar.h"
#include<list>

class COneal : public BaseVector
{
public:

	//アニメーションフレーム数
	static constexpr auto ONEAL_ANIM_FRAME = 8;

	//画像の数
	static constexpr auto ONEAL_IMG_NUM = 5;

	//アニメーション数
	static constexpr auto ONEAL_ANIM_NUM = 4;

	//アニメーションの順番
	//static constexpr int ONEAL_ANIM_ORDER[ONEAL_ANIM_NUM] = { 0,1,2,1 };

	//移動速度
	static constexpr auto ONEAL_SPEED = 2.0f;

public:

	//画像ハンドル、システム上の座標
	COneal(Point, MapPoint);
	~COneal();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//アニメーション処理
	void Anim(int animMax, int* index, bool loop);

public:

private:

	//画像ハンドル
	int ImgHandle[ONEAL_IMG_NUM]{ 0,0,0,0,0 };

	//死亡画像ハンドル
	int DeadImgHandle[4]{ 0,0,0,0 };

	//アニメーションの画像インデックス
	int AnimIndex{ 0 };
	//アニメーションカウント
	int AnimCnt{ ONEAL_ANIM_FRAME };

	//プレイヤーとの差分
	float Distance{ 0.0f };

	//死亡フラグ
	bool IsDead{ false };

	//死亡カウント
	int DeadCnt{ 0 };

	//ストップカウント
	int StopCnt{ 0 };

	list<Cell> vec_last_route;

	int move_cnt{ 0 };
};