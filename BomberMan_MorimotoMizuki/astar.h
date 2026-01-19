// A-Star探索用プログラム
// DXライブラKcomテンプレート用
// 2次元マップの最短ルート検索用


//astar.h
#pragma once
#include "objBase.h"
#include <vector>

#include <list>
#include <queue>
#include <cmath>
#include <cfloat>
#include <cstdlib>

using namespace std;

template<size_t H,size_t W>
vector<vector<int>> ArrToVec(int(&arr)[H][W]) {
	vector<vector<int>> result;
	result.reserve(H);

	for (size_t i = 0; i < H; i++) {
		result.emplace_back(arr[i], arr[i] + W);
	}
	return result;
}

// 削除結果
enum EraseResult
{
	NotFound,		// 未発見
	Erased,			// 削除
	CouldntErased	// 削除できない
};
//２次元管理用(x,y)
struct Cell {
	int X, Y;

	Cell() { X = -1; Y = -1; };
	Cell(int x, int y) {
		X = x;
		Y = y;
	}
};
//ノード
struct Node
{
	Node() :Node(0, 0) {}

	Node(int x, int y) :
		pos(x, y),
		HeuristicCost(9999),
		TotalCost(0)
	{
		AdjucentNodes.clear();
	}

	Cell pos;//ノード座標
	vector<Node*> AdjucentNodes;//隣接するノード
	float HeuristicCost;//ヒューリスティックコスト
	float TotalCost;//コスト(ヒューリスティックコスト込み)
};

// 昇順ソート用関数
bool Less(Node*, Node*);
//比較
bool IsEqualCell(const Cell&, const Cell&);
//ノード削除
EraseResult EraseNode(list<Node*>&, Node*, float);
//ノードに追加
bool AddAdjacentNode(list<Node*>&, list<Node*>&, Node*, float);
// セル範囲チェック関数
bool IsCellWithinTheRange(int, int);
bool IsCellWithinTheRange(int, int, int, int);
//ルート計算(スタート位置,ゴール位置)
list<Cell> ROUTE_CALCULATION(Cell, Cell);
//ルート計算(マップサイズx,マップサイズy,スタート位置,ゴール位置,マップデータ配列2次元を1次元化したもの)
//list<Cell> ROUTE_CALCULATION(int, int, Cell, Cell, int*);
//ルート計算(マップサイズx,マップサイズy,スタート位置,ゴール位置,マップデータvector型)
list<Cell> ROUTE_CALCULATION2(int, int, Cell, Cell, vector<vector<int>>,vector<Obj_Id> cost_obj_id); // <- vector<Obj_Id> cost_obj_id 変更
