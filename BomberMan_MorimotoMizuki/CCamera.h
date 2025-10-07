#pragma once
#include "objBase.h"

class CCamera {
public:
	CCamera();

	void Action();
	void Action(Base3D*, list<unique_ptr<Base3D>>&);//指定したオブジェクトを追跡
	void Action(list<unique_ptr<Base3D>>&);

	//追跡する指定オブジェクトに対しての基準となるカメラの高さ
	float LOOK_AT_HEIGHT{ 2.5 };
	//追跡する指定オブジェクトに対してのカメラ距離
	float LOOK_AT_DISTANCE{ 10 };

	Point3 pos{ 0,0,0 };//カメラの位置
	Point3 target{ 0,0,0 };//カメラの注視点

	int mouseX{ 0 }, mouseY{ 0 };//マウスの位置
	int tmp_mouseX{ 0 }, tmp_mouseY{ 0 };//マウスの位置保存用

	float horizontal_angle{ 0 };//xy平面角(UpVecY)
	float vertical_angle{ 0 };//yz平面角(UpVecY)

	void Draw();//カメラ情報描画

	~CCamera();
};