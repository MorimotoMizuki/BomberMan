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

	//クラッシュブロックの生成から除外する座標
	static constexpr MapPoint ExclusionPoint[3] = {
		{1,1},
		{2,1},
		{1,2}
	};

	////ステージのアイテムの数 : (火力, 爆弾, リモコン, ブーツ, 爆弾通過, 壁通過, 火炎バリア, パーフェクトマン)
	//static constexpr int StageItemNum[STAGE_SUM][ITEM_SUM] = {
	//	{1},
	//	{1},
	//	{1},
	//};

private:

	//ステージの敵のデータ
	std::vector<int> stage_enemy_data;

	//ステージのアイテムのデータ
	std::vector<int> stage_item_data;

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

	//敵のデータ読み込み
	std::vector<int> LoadEnemyData(int stage_num);
	//アイテムのデータ読み込み
	std::vector<int> LoadItemData(int stage_num);

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
	std::vector<std::vector<int>> SetRandomItemInMap(std::vector<int> stage_item_data);

	//マップに敵をランダムで生成 : 敵の出現数の配列
	void SetRandomEnemy(std::vector<int> enemy_data);

	//ステージごとの敵の合計数を取得
	int GetStageEnemyTotal();

	//ステージごとのアイテムの合計数を取得
	int GetStageItemTotal(int stageNum);
};