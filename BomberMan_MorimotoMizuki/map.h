#pragma once

#include"objBase.h"

#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>

class CMap 
{
public:

	//クラッシュブロックが生成される確率 1 = 10%
	static constexpr auto CREATE_CRASH_BLOCK_PROBABILITY = 3;

	//クラッシュブロックの生成から除外する座標
	static constexpr MapPoint ExclusionPoint[3] = {
		{1,1},
		{2,1},
		{1,2}
	};

	//ステージの敵の数 : (バロム, )
	static constexpr int StageEnemyNum[STAGE_SUM][1] = {
		{6},
		{3},
		{3},
	};


private:

	//マップチップ画像
	int img;

	//マップデータ保存用 : + 1 は画面から余分に開けておく
	int map[MAP_CHIP_H + 1][MAP_CHIP_W + 1]{ 0 };

	//カメラ座標
	Point camera_pos{ 0,0 };

	//クラッシュブロックの数
	int CrashBlockNum{ 0 };

public:
	CMap();
	~CMap() { DeleteGraph(img); };

	//マップデータ読み込み
	void LoadMap();
	//マップ生成
	void Map_Obj_Creation(vector<unique_ptr<BaseVector>>&);
	//マップ更新処理
	void Action(vector<unique_ptr<BaseVector>>&);

	//マップにクラッシュブロックをランダムで設定(クラッシュブロックが生成される確率)
	void SetRandomCrashBlockInMap(int);

	//マップに扉(ゴール)をランダムで設定
	int SetRandomDoorInMap();

	//マップに敵をランダムで生成 : (バロム, )
	void SetRandomEnemy(int);

	//ステージごとの敵の合計数を取得
	int GetStageEnemyTotal(int stageNum);
};