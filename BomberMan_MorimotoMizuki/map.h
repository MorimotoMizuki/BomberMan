#pragma once

#include"objBase.h"

#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<map>

class CMap 
{
public:

	//クラッシュブロックが生成される確率 1 = 10%
	static constexpr auto CREATE_CRASH_BLOCK_PROBABILITY = 3;

	//アイテムの数
	static constexpr auto ITEM_SUM = 1;

	//クラッシュブロックの生成から除外する座標
	static constexpr MapPoint ExclusionPoint[3] = {
		{1,1},
		{2,1},
		{1,2}
	};

	//ステージの敵の数 : (バロム, オニール)
	static constexpr int StageEnemyNum[STAGE_SUM][2] = {
		{6, 1},
		{3, 0},
		{3, 0},
	};

	//ステージのアイテムの数 : (火力, 爆弾, リモコン, ブーツ, 爆弾通過, 壁通過, 火炎バリア, パーフェクトマン)
	static constexpr int StageItemNum[STAGE_SUM][ITEM_SUM] = {
		{1},
		{1},
		{1},
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

	//ドアを生成するクラッシュブロックの番号
	int SetDoorNum{ 0 };

	//アイテムの生成情報配列
	std::vector<std::vector<int>> SetItemNum;

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

	//マップにアイテムをランダムで設定
	void SetRandomItemInMap();

	//マップに敵をランダムで生成 : (バロム, オニール)
	void SetRandomEnemy(int, int);

	//ステージごとの敵の合計数を取得
	int GetStageEnemyTotal(int stageNum);

	//ステージごとのアイテムの合計数を取得
	int GetStageItemTotal(int stageNum);
};