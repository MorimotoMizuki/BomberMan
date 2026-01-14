#pragma once

#include"objBase.h"

class CDoor : public BaseVector
{
public:

	static constexpr auto GOAL_IN_DISTANCE = 6;


public:

	//座標、システム上の座標
	CDoor(Point, MapPoint);
	~CDoor();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//扉爆破処理
	void DoorExplosion(vector<unique_ptr<BaseVector>>&);

public:

	//ドアの開放フラグ
	bool IsOpen{ false };

	//ドア爆破フラグ
	bool IsDoorExplosion{ false };

private:

	//プレイヤーとの差分
	float Distance{ 0.0f };

	//SE ステージクリア用
	int SE_StageClear{ -1 };

	//ドア爆破カウント
	int ExplosionCnt{ 0 };
};