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

private:

	//システム上の座標
	MapPoint SystemPos{ 0,0 };

	//プレイヤーとの差分
	float Distance{ 0.0f };

};