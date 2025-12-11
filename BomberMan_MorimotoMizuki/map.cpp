#include"map.h"
#include"obj.h"
#include <numeric>
#include <iterator>
#include"function.h"
#include <algorithm>
#include <unordered_set>

//文字列分割関数 : 指定した文字で文字列を分割する
//(文字列, 分割文字)
vector<string> split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;

	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

CMap::CMap() 
{
	//マップチップ画像読み込み
	img = LoadGraph("image\\map_chip.png");

	SetItemNum.resize(ITEM_SUM);
}

//マップデータ読み込み
void CMap::LoadMap()
{
	ifstream fp("image\\map_data.csv");
	string str; //読み込んだ

	if (!fp.fail())
	{
		for (int y = 0; y < MAP_CHIP_H; y++)
		{
			getline(fp, str);//1行読み込み
			vector<string> strv = split(str, ',');//カンマで分割
			for (int x = 0; x < MAP_CHIP_W; x++)
			{
				map[y][x] = stoi(strv.at(x));//文字列を数値に変換して保存
				gNowMap[y][x] = map[y][x];
			}
		}

		fp.close();//ファイルを閉じる
	}
}

//マップ生成
void CMap::Map_Obj_Creation(vector<unique_ptr<BaseVector>>& base)
{
	//クラッシュブロックをランダムで設定
	SetRandomCrashBlockInMap(CREATE_CRASH_BLOCK_PROBABILITY);

	//敵を生成
	SetRandomEnemy(StageEnemyNum[gNowStageNum - 1][0], StageEnemyNum[gNowStageNum - 1][1]);

	//ドアを設置する数を設定
	SetDoorNum = SetRandomDoorInMap();

	SetRandomItemInMap();

	int crashBlockNum = 0;
	bool isDoorSet = false;
	int itemIndex = 0;
	int item_id = -1;

	//すでに使われた cblockNum を記録する
	std::unordered_set<int> used;

	for (int y = 0; y < MAP_CHIP_H; y++)
	{
		for (int x = 0; x < MAP_CHIP_W; x++)
		{
			Point p{ x * CHIP_SIZE, y * CHIP_SIZE + WINDOW_HEADER };
			MapPoint s_p{ x, y };

			int point = gNowMap[y][x];
			bool isBreak{ false };
			//アイテムID登録
			for (int i = 0; i < ITEM_SUM; i++){
				for (int cblockNum : SetItemNum[i]){

					//すでに選ばれた場合は無視
					if (used.count(cblockNum))continue;

					if (cblockNum == crashBlockNum){
						item_id = i;
						isBreak = true;
						break;
					}
					else{
						item_id = -1;
					}
				}
				if (isBreak) break;
			}

			switch (point)
			{
			//ブロック
			case 0:
				item_id = -1;
				base.emplace_back((unique_ptr<BaseVector>) new CBlock(p, s_p, gNowMap[y][x], img, static_cast<Item_Id>(item_id)));
				break;
			//クラッシュブロック
			case 1:
				base.emplace_back((unique_ptr<BaseVector>) new CBlock(p, s_p, gNowMap[y][x], img, static_cast<Item_Id>(item_id)));
				//アイテム生成
				if (item_id != -1) {
					base.emplace_back((unique_ptr<BaseVector>) new CItem(p, s_p, static_cast<Item_Id>(item_id)));
					item_id = -1;
					used.insert(crashBlockNum); //使用済みに登録
				}
				crashBlockNum++;
				break;
			//バロム
			case Enemy_Id::BALLOM:
				base.emplace_back((unique_ptr<BaseVector>) new CBallom(p, s_p));
				gNowMap[y][x] = -1;
				break;
			//オニール
			case Enemy_Id::ONEAL:
				base.emplace_back((unique_ptr<BaseVector>) new COneal(p, s_p));
				gNowMap[y][x] = -1;
				break;
			default:
				break;
			}

			//ドアを生成
			if (SetDoorNum == crashBlockNum && !isDoorSet) {
				base.emplace_back((unique_ptr<BaseVector>) new CDoor(p, s_p));
				isDoorSet = true;
			}
		}
	}
}

//マップにクラッシュブロックをランダムで設定
void CMap::SetRandomCrashBlockInMap(int probability)
{
	CrashBlockNum = 0;
	int randomNum = 0;
	for (int y = 0; y < MAP_CHIP_H; y++)
	{
		for (int x = 0; x < MAP_CHIP_W; x++)
		{
			randomNum = Range_Random_Number(1, 10);
			//空白　かつ　一定の確率の場合
			if (gNowMap[y][x] == -1 && randomNum <= probability)
			{
				//設定した除外座標だった場合はスキップ
				if ((x == ExclusionPoint[0].x && y == ExclusionPoint[0].y) ||
					(x == ExclusionPoint[1].x && y == ExclusionPoint[1].y) ||
					(x == ExclusionPoint[2].x && y == ExclusionPoint[2].y))
					continue;

				gNowMap[y][x] = 1;
				CrashBlockNum++;
			}
		}
	}
}

//マップに扉(ゴール)をランダムで設定
int CMap::SetRandomDoorInMap()
{
	int setDoorNum = Range_Random_Number(1, CrashBlockNum);
	return setDoorNum;
}

//マップにアイテムをランダムで設定
void CMap::SetRandomItemInMap()
{
	int itemTotal = GetStageItemTotal(gNowStageNum - 1);
	int itemIndex = 0;

	//すでに使われた itemNum を記録する
	std::unordered_set<int> used;

	//火力
	for (int i = 0; i < ITEM_SUM; i++)
	{
		itemIndex = 0;
		while (itemIndex < StageItemNum[gNowStageNum - 1][i])
		{
			int itemNum = Range_Random_Number(1, CrashBlockNum);

			//すでに選ばれた場合は無視
			if (used.count(itemNum))continue;

			if (itemNum != SetDoorNum) {
				SetItemNum[i].push_back(itemNum);
				used.insert(itemNum); //使用済みに登録
				itemIndex++;
			}
		}
	}

	//昇順にソート
	for (auto& itemList : SetItemNum) {
		std::sort(itemList.begin(), itemList.end());
	}
}

//マップ更新処理
void CMap::Action(vector<unique_ptr<BaseVector>>& base)
{
	//CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);

	////カメラをプレイヤー座標
	//camera_pos = p->m_pos;

	////カメラの領域判定
	////左端
	//if (camera_pos.x < DRAW_CHIP_W * CHIP_SIZE / 2)
	//	camera_pos.x = DRAW_CHIP_W * CHIP_SIZE / 2;
	////右端
	//if (camera_pos.x > MAP_CHIP_W * CHIP_SIZE - DRAW_CHIP_W * CHIP_SIZE / 2 - CHIP_SIZE)
	//	camera_pos.x = MAP_CHIP_W * CHIP_SIZE - DRAW_CHIP_W * CHIP_SIZE / 2 - CHIP_SIZE;

	////上端
	//if (camera_pos.y < DRAW_CHIP_H * CHIP_SIZE / 2)
	//	camera_pos.y = DRAW_CHIP_H * CHIP_SIZE / 2;
	////下端
	//if (camera_pos.y > MAP_CHIP_H * CHIP_SIZE - DRAW_CHIP_H * CHIP_SIZE / 2)
	//	camera_pos.y = MAP_CHIP_H * CHIP_SIZE - DRAW_CHIP_H * CHIP_SIZE / 2;

	////マップチップの描画更新処理
	////現在のチップオブジェクトを削除
	//Del_ListObj(base, BLOCK);

	////マップ上のカメラの左上の座標
	//Point m_camera{ DRAW_CHIP_W * CHIP_SIZE / 2 - camera_pos.x,
	//				DRAW_CHIP_H * CHIP_SIZE / 2 - camera_pos.y
	//};

	////描画開始する位置
	//int ChipX = -(m_camera.x / CHIP_SIZE);
	//int ChipY = -(m_camera.y / CHIP_SIZE);

	////描画開始位置
	//Point DrawPos{ m_camera.x + ChipX * CHIP_SIZE,
	//				m_camera.y + ChipY * CHIP_SIZE + +WINDOW_HEADER
	//};

	//Point disPos{ p->m_pos.x - p->pos.x, -WINDOW_HEADER };

	//if (p->pos.x < 480)
	//	disPos = { 0.0f,-WINDOW_HEADER };
	//else if (p->pos.x > 480)
	//	disPos = { 960.0f,-WINDOW_HEADER };

	////マップチップを生成
	//Point chipPos{ DrawPos.x, DrawPos.y };	//描画位置
	//for (int y = 0; y < DRAW_CHIP_H; y++)
	//{
	//	chipPos.x = DrawPos.x;
	//	for (int x = 0; x < DRAW_CHIP_W + 2; x++)
	//	{
	//		MapPoint s_p{ (chipPos.x + disPos.x) / 64, (chipPos.y + disPos.y) / 64 };
	//		//空白　かつ　爆弾　ではない場合
	//		if (gNowMap[y + ChipY][x + ChipX] != -1 && gNowMap[y + ChipY][x + ChipX] != 5)
	//		{
	//			//ブロック生成
	//			base.emplace_back((unique_ptr<BaseVector>)new CBlock(chipPos, s_p, map[y + ChipY][x + ChipX], img));
	//		}
	//		chipPos.x += CHIP_SIZE;
	//	}
	//	chipPos.y += CHIP_SIZE;
	//}

	////プレイヤーの描画位置計算
	//{
	//	p->pos.x = p->m_pos.x - camera_pos.x + DRAW_CHIP_W * CHIP_SIZE / 2;
	//	p->pos.y = p->m_pos.y - camera_pos.y + DRAW_CHIP_H * CHIP_SIZE / 2;
	//}
}

//マップに敵をランダムで生成 : (バロム, )
void CMap::SetRandomEnemy(int ballomNum, int onealNum)
{
	int ballomCnt{ 0 };
	int onealCnt{ 0 };

	while (ballomCnt != ballomNum)
	{
		int ballomX = Range_Random_Number(1, MAP_CHIP_W - 1);
		int ballomY = Range_Random_Number(1, MAP_CHIP_H - 1);

		//設定した除外座標だった場合はコンテニュー
		if ((ballomX == ExclusionPoint[0].x && ballomY == ExclusionPoint[0].y) ||
			(ballomX == ExclusionPoint[1].x && ballomY == ExclusionPoint[1].y) ||
			(ballomX == ExclusionPoint[2].x && ballomY == ExclusionPoint[2].y))
			continue;

		if (gNowMap[ballomY][ballomX] == -1)
		{
			gNowMap[ballomY][ballomX] = Enemy_Id::BALLOM;
			ballomCnt++;
		}
		else
			continue;
	}

	while (onealCnt != onealNum)
	{
		int onealX = Range_Random_Number(1, MAP_CHIP_W - 1);
		int onealY = Range_Random_Number(1, MAP_CHIP_H - 1);

		//設定した除外座標だった場合はコンテニュー
		if ((onealX == ExclusionPoint[0].x && onealY == ExclusionPoint[0].y) ||
			(onealX == ExclusionPoint[1].x && onealY == ExclusionPoint[1].y) ||
			(onealX == ExclusionPoint[2].x && onealY == ExclusionPoint[2].y))
			continue;

		if (gNowMap[onealY][onealX] == -1)
		{
			gNowMap[onealY][onealX] = Enemy_Id::ONEAL;
			onealCnt++;
		}
		else
			continue;
	}
}

//ステージごとの敵の合計数を取得
int CMap::GetStageEnemyTotal(int stageNum)
{
	return std::accumulate(std::begin(StageEnemyNum[stageNum]), std::end(StageEnemyNum[stageNum]), 0);
}

//ステージごとのアイテムの合計数を取得
int CMap::GetStageItemTotal(int stageNum)
{
	return std::accumulate(std::begin(StageItemNum[stageNum]), std::end(StageItemNum[stageNum]), 0);
}