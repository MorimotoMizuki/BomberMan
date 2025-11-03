#pragma once

#include"objBase.h"

#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>

class CMap {

private:

	//マップチップ画像
	int img;

	//マップデータ保存用 : + 1 は画面から余分に開けておく
	int map[MAP_CHIP_H + 1][MAP_CHIP_W + 1]{ 0 };

	//カメラ座標
	Point camera_pos{ 0,0 };

public:
	CMap();
	~CMap() {};

	//マップデータ読み込み
	void LoadMap();
	//マップ生成
	void Map_Obj_Creation(vector<unique_ptr<BaseVector>>&);
	//マップ更新処理
	void Action(vector<unique_ptr<BaseVector>>&);

};