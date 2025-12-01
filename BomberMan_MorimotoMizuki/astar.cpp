//A-Star
#pragma once
#include "astar.h"

// 昇順ソート用関数
bool Less(Node* a, Node* b)
{
	if (a->TotalCost < b->TotalCost)
	{
		return true;
	}

	return false;
}
//比較
bool IsEqualCell(const Cell& a, const Cell& b)
{
	if (a.X == b.X &&
		a.Y == b.Y)
	{
		return true;
	}

	return false;
}
//ノード削除
EraseResult EraseNode(list<Node*>& close_list, Node* new_node, float new_cost)
{
	// クローズリストチェック
	for (auto itr = close_list.begin(); itr != close_list.end(); itr++)
	{
		// ノードと同じセルがあるか調べる
		if (IsEqualCell(new_node->pos, (*itr)->pos) == true)
		{
			// コストの比較
			if (new_cost < (*itr)->TotalCost)
			{
				close_list.erase(itr);
				return EraseResult::Erased;
			}
			else
			{
				return EraseResult::CouldntErased;
			}
		}
	}

	return EraseResult::NotFound;
}
//ノードに追加
bool AddAdjacentNode(list<Node*>& open_list, list<Node*>& close_list, Node* adjacent_node, float cost)
{
	bool can_update = true;

	std::list<Node*> node_list[] =
	{
		close_list,
		open_list
	};

	for (std::list<Node*>& list : node_list)
	{
		// リストに同じノードがあってリストの方のコストが高いなら削除
		if (EraseNode(list, adjacent_node, cost) == EraseResult::CouldntErased)
		{
			can_update = false;
		}
	}

	if (can_update == true)
	{
		open_list.push_back(adjacent_node);
		return true;
	}

	return false;
}
// セル範囲チェック関数
bool IsCellWithinTheRange(int x, int y)
{
	if (x >= 0 &&
		x < MAP_CHIP_W &&
		y >= 0 &&
		y < MAP_CHIP_H)
	{
		return true;
	}

	return false;
}
bool IsCellWithinTheRange(int x, int y, int mapX, int mapY)
{
	if (x >= 0 &&
		x < mapX &&
		y >= 0 &&
		y < mapY)
	{
		return true;
	}

	return false;
}


//ルート計算(マップサイズx,マップサイズy,スタート位置,ゴール位置,vector型二次元マップ)
list<Cell> ROUTE_CALCULATION2(int MX, int MY, Cell s, Cell g, vector<vector<int>> mp) {
	//ノード計算用
	vector<vector<Node>> Graph;
	Graph.resize(MY);
	for (int i = 0; i != MY; i++) {
		Graph[i].resize(MX);
	}

	//隣接ノードの作成(初期化)
	for (int y = 0; y < MY; y++) {
		for (int x = 0; x < MX; x++) {
			Graph[y][x].pos.X = x;
			Graph[y][x].pos.Y = y;
			Cell adjacent_[] =
			{
				Cell(x,y - 1),
				Cell(x - 1,y),
				Cell(x + 1,y),
				Cell(x,y + 1)
			};
			//隣接ノードの作成
			for (const Cell& c : adjacent_)
			{
				//マップ外の場合は、作成しない。
				if (IsCellWithinTheRange(c.X, c.Y, MX, MY) && mp[c.Y][c.X] != 1) {
					Graph[y][x].AdjucentNodes.push_back(&Graph[c.Y][c.X]);
				}
			}
		}
	}

	list<Cell> last_route;//最終経路
	list<Node*> open_list;//オープンノード
	list<Node*> close_list;//クローズノード

	//ＧＯＡＬノードの定義
	const Node* goal_node = &Graph[g.Y][g.X];

	//更新ノード位置保存用
	Cell last_update[MAP_CHIP_H][MAP_CHIP_W];
	
	vector<vector<Cell>> last_up;
	last_up.resize(MY);
	for (int i = 0; i < MY; i++) {
		last_up[i].resize(MX);
	}
	
	//グラフの初期化
	for (int y = 0; y < MY; y++) {
		for (int x = 0; x < MX; x++) {
			Graph[y][x].HeuristicCost = 9999;
			Graph[y][x].TotalCost = 0;
		}
	}

	//スタートノードの指定(オープンリスト)
	open_list.push_back(&Graph[s.Y][s.X]);

	//オープンリストがなくなるまで検索
	while (!open_list.empty()) {
		Node* search_node = (*open_list.begin());
		open_list.erase(open_list.begin());//探索リストから除外

		//ゴールに到達したら終わり
		if (search_node->pos.X == g.X && search_node->pos.Y == g.Y) {
			close_list.push_back(search_node);//クローズリストに最後のノードを追加
			break;
		}

		//隣接ノードのチェック
		for (Node* adjacent_node : search_node->AdjucentNodes) {
			//ヒューリスティックコストの計算
			if (adjacent_node->HeuristicCost == 9999) {//初期値の場合だけ計算
				//ノードからゴールまでの距離を求める(ユークリッド距離)
				float x = fabsf(g.X - adjacent_node->pos.X);
				float y = fabsf(g.Y - adjacent_node->pos.Y);
				adjacent_node->HeuristicCost =
					sqrtf(x * x + y * y);
			}

			//ノード間コスト
			float edge_cost = mp[adjacent_node->pos.Y][adjacent_node->pos.X];
			if (edge_cost >= 1) edge_cost = 1;//基本的に通路以外はコスト'1'

			//取得ノードのトータルコスト
			float node_cost = search_node->TotalCost;
			//トータルコスト計算
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			//ノード追加
			{
				if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost))
				{
					adjacent_node->TotalCost = total_cost;//トータルコスト更新
					//経路を更新したセルに保存
					last_update[adjacent_node->pos.Y][adjacent_node->pos.X] = search_node->pos;
				}
			}
			//コストの表示
			DrawFormatString(adjacent_node->pos.X * 32 + 512, adjacent_node->pos.Y * 32, GetColor(255, 255, 255), "%.0lf", adjacent_node->TotalCost);

		}

		bool is_add_close{ true };

		//クローズリストチェック
		for (auto i = close_list.begin(); i != close_list.end(); i++) {
			//ノードと同じセルがあるか調べる
			if (IsEqualCell(search_node->pos, (*i)->pos)) {
				is_add_close = false;
				break;
			}
		}

		//同じノードがなかったら追加して探索終了
		if (is_add_close) {
			close_list.push_back(search_node);
		}

		//ソート
		open_list.sort(Less);
	}

	//経路
	list<Cell> route_list;

	//ゴールから復元
	route_list.push_back(g);
	while (!route_list.empty()) {
		Cell route = route_list.front();
		//スタートセルなら処理終了
		if (IsEqualCell(route, s) == true) {
			last_route.swap(route_list);
			break;
		}
		else {
			if (route.Y >= 0 && route.Y <= MY && route.X >= 0 && route.X <= MX) {
				//追加
				route_list.push_front(last_update[route.Y][route.X]);
			}
			else {
				//経路がない
				break;
			}
		}
	}

	return last_route;
}

/*
//ルート計算(マップサイズx,マップサイズy,スタート位置,ゴール位置,マップデータ配列2次元を1次元化したもの)
list<Cell> ROUTE_CALCULATION(int MX, int MY, Cell s, Cell g, int* _mp) {
	//計算用マップ
	vector<vector<int>> mp;
	mp.resize(MY);
	for (int i = 0; i != MY; i++) {
		mp[i].resize(MX);
	}
	//マップを1次元から2次元に変換
	int x = 0, y = 0;
	for (int i = 0; i < MX * MY; i++) {
		mp[y][x] = *(_mp + i);
		x++;
		if (x >= MX) {
			x = 0;
			y++;
		}
	}

	//ノード計算用
	vector<vector<Node>> Graph;
	Graph.resize(MY);
	for (int i = 0; i != MY; i++) {
		Graph[i].resize(MX);
	}

	//隣接ノードの作成(初期化)
	for (int y = 0; y < MAP_CHIP_HEIGHT; y++) {
		for (int x = 0; x < MAP_CHIP_WIDTH; x++) {
			Graph[y][x].pos.X = x;
			Graph[y][x].pos.Y = y;
			Cell adjacent_[] =
			{
				Cell(x,y - 1),
				Cell(x - 1,y),
				Cell(x + 1,y),
				Cell(x,y + 1)
			};
			//隣接ノードの作成
			for (const Cell& c : adjacent_)
			{
				//マップ外の場合は、作成しない。
				if (IsCellWithinTheRange(c.X, c.Y, MX, MY) && mp[c.Y][c.X] != 1) {
					Graph[y][x].AdjucentNodes.push_back(&Graph[c.Y][c.X]);
				}
			}
		}
	}

	list<Cell> last_route;//最終経路
	list<Node*> open_list;//オープンノード
	list<Node*> close_list;//クローズノード

	//ＧＯＡＬノードの定義
	const Node* goal_node = &Graph[g.Y][g.X];

	//更新ノード位置保存用
	Cell last_update[MAP_CHIP_HEIGHT][MAP_CHIP_WIDTH];

	//グラフの初期化
	for (int y = 0; y < MAP_CHIP_HEIGHT; y++) {
		for (int x = 0; x < MAP_CHIP_WIDTH; x++) {
			Graph[y][x].HeuristicCost = 9999;
			Graph[y][x].TotalCost = 0;
		}
	}

	//スタートノードの指定(オープンリスト)
	open_list.push_back(&Graph[s.Y][s.X]);

	//オープンリストがなくなるまで検索
	while (!open_list.empty()) {
		Node* search_node = (*open_list.begin());
		open_list.erase(open_list.begin());//探索リストから除外

		//ゴールに到達したら終わり
		if (search_node->pos.X == g.X && search_node->pos.Y == g.Y) {
			close_list.push_back(search_node);//クローズリストに最後のノードを追加
			break;
		}

		//隣接ノードのチェック
		for (Node* adjacent_node : search_node->AdjucentNodes) {
			//ヒューリスティックコストの計算
			if (adjacent_node->HeuristicCost == 9999) {//初期値の場合だけ計算
				//ノードからゴールまでの距離を求める(ユークリッド距離)
				float x = fabsf(g.X - adjacent_node->pos.X);
				float y = fabsf(g.Y - adjacent_node->pos.Y);
				adjacent_node->HeuristicCost =
					sqrtf(x * x + y * y);
			}

			//ノード間コスト
			float edge_cost = mp[adjacent_node->pos.Y][adjacent_node->pos.X];
			//取得ノードのトータルコスト
			float node_cost = search_node->TotalCost;
			//トータルコスト計算
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			//ノード追加
			{
				if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost))
				{
					adjacent_node->TotalCost = total_cost;//トータルコスト更新
					//経路を更新したセルに保存
					last_update[adjacent_node->pos.Y][adjacent_node->pos.X] = search_node->pos;
				}
			}
			//コストの表示
			DrawFormatString(adjacent_node->pos.X * 32 + 512, adjacent_node->pos.Y * 32, GetColor(255, 255, 255), "%.0lf", adjacent_node->TotalCost);

		}

		bool is_add_close{ true };

		//クローズリストチェック
		for (auto i = close_list.begin(); i != close_list.end(); i++) {
			//ノードと同じセルがあるか調べる
			if (IsEqualCell(search_node->pos, (*i)->pos)) {
				is_add_close = false;
				break;
			}
		}

		//同じノードがなかったら追加して探索終了
		if (is_add_close) {
			close_list.push_back(search_node);
		}

		//ソート
		open_list.sort(Less);
	}

	//経路
	list<Cell> route_list;

	//ゴールから復元
	route_list.push_back(g);
	while (!route_list.empty()) {
		Cell route = route_list.front();
		//スタートセルなら処理終了
		if (IsEqualCell(route, s) == true) {
			last_route.swap(route_list);
			break;
		}
		else {
			if (route.Y >= 0 && route.Y <= MY && route.X >= 0 && route.X <= MX) {
				//追加
				route_list.push_front(last_update[route.Y][route.X]);
			}
			else {
				//経路がない
				break;
			}
		}
	}

	return last_route;
}
*/

/*
//ルート計算(スタート位置,ゴール位置)
list<Cell> ROUTE_CALCULATION(Cell s, Cell g) {
	//計算用ノード
	Node Graph[MAP_CHIP_HEIGHT][MAP_CHIP_WIDTH];
	//隣接ノードの作成(初期化)
	for (int y = 0; y < MAP_CHIP_HEIGHT; y++) {
		for (int x = 0; x < MAP_CHIP_WIDTH; x++) {
			Graph[y][x].pos.X = x;
			Graph[y][x].pos.Y = y;
			Cell adjacent_[] =
			{
				Cell(x,y - 1),
				Cell(x - 1,y),
				Cell(x + 1,y),
				Cell(x,y + 1)
			};
			//隣接ノードの作成
			for (const Cell& c : adjacent_)
			{
				if (IsCellWithinTheRange(c.X, c.Y) && map[c.Y][c.X] == 0) {
					Graph[y][x].AdjucentNodes.push_back(&Graph[c.Y][c.X]);
				}
			}
		}
	}

	list<Cell> last_route;//最終経路
	list<Node*> open_list;//オープンノード
	list<Node*> close_list;//クローズノード

	//ＧＯＡＬノードの定義
	const Node* goal_node = &Graph[g.Y][g.X];

	//更新ノード位置保存用
	Cell last_update[MAP_CHIP_HEIGHT][MAP_CHIP_WIDTH];

	//グラフの初期化
	for (int y = 0; y < MAP_CHIP_HEIGHT; y++) {
		for (int x = 0; x < MAP_CHIP_WIDTH; x++) {
			Graph[y][x].HeuristicCost = 9999;
			Graph[y][x].TotalCost = 0;
		}
	}

	//スタートノードの指定(オープンリスト)
	open_list.push_back(&Graph[s.Y][s.X]);
	//オープンリストがなくなるまで検索
	while (!open_list.empty()) {
		Node* search_node = (*open_list.begin());
		open_list.erase(open_list.begin());//探索リストから除外

		//ゴールに到達したら終わり
		if (search_node->pos.X == g.X && search_node->pos.Y == g.Y) {
			close_list.push_back(search_node);//クローズリストに最後のノードを追加
			break;
		}

		//隣接ノードのチェック
		for (Node* adjacent_node : search_node->AdjucentNodes) {
			//ヒューリスティックコストの計算
			if (adjacent_node->HeuristicCost == 999) {//初期値の場合だけ計算
				//ノードからゴールまでの距離を求める
				float x = fabsf(g.X - adjacent_node->pos.X);
				float y = fabsf(g.Y - adjacent_node->pos.Y);
				adjacent_node->HeuristicCost =
					sqrtf(x * x + y * y);
			}

			//ノード間コスト
			float edge_cost = map[adjacent_node->pos.Y][adjacent_node->pos.X];
			//取得ノードのトータルコスト
			float node_cost = search_node->TotalCost;
			//トータルコスト計算
			float total_cost = edge_cost + adjacent_node->HeuristicCost + node_cost;

			//ノード追加
			{
				if (AddAdjacentNode(open_list, close_list, adjacent_node, total_cost))
				{
					adjacent_node->TotalCost = total_cost;//トータルコスト更新
					//経路を更新したセルに保存
					last_update[adjacent_node->pos.Y][adjacent_node->pos.X] = search_node->pos;
				}
			}
		}

		bool is_add_close{ true };

		//クローズリストチェック
		for (auto i = close_list.begin(); i != close_list.end(); i++) {
			//ノードと同じセルがあるか調べる
			if (IsEqualCell(search_node->pos, (*i)->pos)) {
				is_add_close = false;
				break;
			}
		}

		//同じノードがなかったら追加して探索終了
		if (is_add_close) {
			close_list.push_back(search_node);
		}

		//ソート
		open_list.sort(Less);

		//経路
		list<Cell> route_list;

		//ゴールから復元
		route_list.push_back(g);
		while (!route_list.empty()) {
			Cell route = route_list.front();
			//スタートセルなら処理終了
			if (IsEqualCell(route, s) == true) {
				last_route.swap(route_list);
				break;
			}
			else {
				if (route.Y >= 0 && route.Y <= MAP_CHIP_HEIGHT && route.X >= 0 && route.X <= MAP_CHIP_WIDTH) {
					//追加
					route_list.push_front(last_update[route.Y][route.X]);
				}
				else {
					//経路がない
					break;
				}
			}
		}
	}
	return last_route;
}
*/