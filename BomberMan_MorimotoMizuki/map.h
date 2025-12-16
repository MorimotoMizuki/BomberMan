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

	// 7 ~ 10 までバグってる

	//ステージの敵の数 : ステージ数,(バロム,オニール,ダル,ミンボー,コンドリア,オバピー,パース,ポンタン)
	static constexpr int StageEnemyNum[STAGE_SUM][ENEMY_VARIATION + 1] = {
		{1,6,1,0,0,0,0,0,0},
		{2,3,3,0,0,0,0,0,0},
		{3,2,2,2,0,0,0,0,0},
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

	//マップに敵をランダムで生成 : 敵の出現数の配列
	void SetRandomEnemy(const int enemy[ENEMY_VARIATION + 1]);

	//ステージごとの敵の合計数を取得
	int GetStageEnemyTotal(int stageNum);

	//ステージごとのアイテムの合計数を取得
	int GetStageItemTotal(int stageNum);
};

/*

STAGE,バロム,オニール,ダル,ミンボー,コンドリア,オバピー,パース,ポンタン
01,6,0,0,0,0,0,0,0
02,3,3,0,0,0,0,0,0
03,2,2,2,0,0,0,0,0
04,1,1,2,2,0,0,0,0
05,0,4,3,0,0,0,0,0
06,0,0,3,2,0,0,0,0
07,0,0,3,3,0,0,0,0
08,0,0,2,4,0,0,0,0
09,0,0,1,4,1,0,0,0
10,0,1,1,1,3,0,0,0
11,0,1,2,3,1,2,0,0
12,0,1,1,1,1,4,0,0
13,0,0,3,3,0,2,0,0
14,0,0,0,0,7,0,1,0
15,0,0,1,3,0,3,1,0
16,0,0,0,3,0,4,1,0
17,0,0,5,1,0,2,1,0
18,3,3,0,0,0,2,2,0
19,1,1,0,0,0,3,2,0
20,0,1,0,1,0,3,0,0
21,0,0,0,0,3,4,2,0
22,0,0,4,3,0,1,1,0
23,0,0,3,3,0,3,1,0
24,0,0,3,2,2,1,0,0
25,0,0,1,3,1,2,0,0
26,1,1,1,1,2,2,1,0
27,1,1,0,1,1,5,1,0
28,0,1,3,3,0,1,1,0
29,0,0,0,0,5,2,2,0
30,0,0,0,2,0,3,4,0
31,0,2,2,2,2,2,0,0
32,0,1,1,3,0,4,1,0
33,0,0,2,2,1,3,2,0
34,0,0,2,3,0,3,2,0
35,0,0,2,1,1,3,2,0
36,0,0,2,2,0,3,3,0
37,0,0,2,1,1,3,3,0
38,0,0,2,2,0,3,3,0
39,0,0,1,1,2,2,4,0
40,0,0,1,2,0,3,4,0
41,0,0,1,1,1,3,4,0
42,0,0,0,1,1,3,5,0
43,0,0,0,1,1,2,6,0
44,0,0,0,1,1,2,6,0
45,0,0,0,0,2,2,6,0
46,0,0,0,0,2,2,6,0
47,0,0,0,0,2,2,6,0
48,0,0,0,0,1,2,6,1
49,0,0,0,0,2,1,6,1
50,0,0,0,0,2,1,5,2

*/