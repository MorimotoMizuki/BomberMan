#include"map.h"
#include"obj.h"

#include"function.h"

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

CMap::CMap() {
	//マップチップ画像読み込み
	img = LoadGraph("image\\map_chip.png");
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

	//ドアを設置する数を設定
	int setDoorNum = SetRandomDoorInMap();
	int crashBlockNum = 0;
	bool isDoorSet = false;

	MapPoint enemyPos{ 3,7 };

	for (int y = 0; y < MAP_CHIP_H; y++)
	{
		for (int x = 0; x < MAP_CHIP_W; x++)
		{
			Point p{ x * CHIP_SIZE, y * CHIP_SIZE + WINDOW_HEADER };
			MapPoint s_p{ x, y };

			int point = gNowMap[y][x];
			switch (point)
			{
			//ブロック
			case 0:
				base.emplace_back((unique_ptr<BaseVector>) new CBlock(p, s_p, gNowMap[y][x], img));
				break;
			case 1:
				base.emplace_back((unique_ptr<BaseVector>) new CBlock(p, s_p, gNowMap[y][x], img));
				crashBlockNum++;
				break;
			default:
				break;
			}

			//ドアを生成
			if (setDoorNum == crashBlockNum && !isDoorSet) {
				base.emplace_back((unique_ptr<BaseVector>) new CDoor(p, s_p));
				isDoorSet = true;
			}

			if(s_p.x == enemyPos.x && s_p.y == enemyPos.y)
				base.emplace_back((unique_ptr<BaseVector>) new CBallom(p, s_p));

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
	int setDoorNum = 0;
	setDoorNum = Range_Random_Number(1, CrashBlockNum);
	return setDoorNum;
}

//マップ更新処理
void CMap::Action(vector<unique_ptr<BaseVector>>& base)
{
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);

	//カメラをプレイヤー座標
	camera_pos = p->m_pos;

	//カメラの領域判定
	//左端
	if (camera_pos.x < DRAW_CHIP_W * CHIP_SIZE / 2)
		camera_pos.x = DRAW_CHIP_W * CHIP_SIZE / 2;
	//右端
	if (camera_pos.x > MAP_CHIP_W * CHIP_SIZE - DRAW_CHIP_W * CHIP_SIZE / 2 - CHIP_SIZE)
		camera_pos.x = MAP_CHIP_W * CHIP_SIZE - DRAW_CHIP_W * CHIP_SIZE / 2 - CHIP_SIZE;

	//上端
	if (camera_pos.y < DRAW_CHIP_H * CHIP_SIZE / 2)
		camera_pos.y = DRAW_CHIP_H * CHIP_SIZE / 2;
	//下端
	if (camera_pos.y > MAP_CHIP_H * CHIP_SIZE - DRAW_CHIP_H * CHIP_SIZE / 2)
		camera_pos.y = MAP_CHIP_H * CHIP_SIZE - DRAW_CHIP_H * CHIP_SIZE / 2;

	//マップチップの描画更新処理
	//現在のチップオブジェクトを削除
	Del_ListObj(base, BLOCK);

	//マップ上のカメラの左上の座標
	Point m_camera{ DRAW_CHIP_W * CHIP_SIZE / 2 - camera_pos.x,
					DRAW_CHIP_H * CHIP_SIZE / 2 - camera_pos.y
	};

	//描画開始する位置
	int ChipX = -(m_camera.x / CHIP_SIZE);
	int ChipY = -(m_camera.y / CHIP_SIZE);

	//描画開始位置
	Point DrawPos{ m_camera.x + ChipX * CHIP_SIZE,
					m_camera.y + ChipY * CHIP_SIZE + +WINDOW_HEADER
	};

	Point disPos{ p->m_pos.x - p->pos.x, -WINDOW_HEADER };

	if (p->pos.x < 480)
		disPos = { 0.0f,-WINDOW_HEADER };
	else if (p->pos.x > 480)
		disPos = { 960.0f,-WINDOW_HEADER };

	//マップチップを生成
	Point chipPos{ DrawPos.x, DrawPos.y };	//描画位置
	for (int y = 0; y < DRAW_CHIP_H; y++)
	{
		chipPos.x = DrawPos.x;
		for (int x = 0; x < DRAW_CHIP_W + 2; x++)
		{
			MapPoint s_p{ (chipPos.x + disPos.x) / 64, (chipPos.y + disPos.y) / 64 };
			//空白　かつ　爆弾　ではない場合
			if (gNowMap[y + ChipY][x + ChipX] != -1 && gNowMap[y + ChipY][x + ChipX] != 5)
			{
				//ブロック生成
				base.emplace_back((unique_ptr<BaseVector>)new CBlock(chipPos, s_p, map[y + ChipY][x + ChipX], img));
			}
			chipPos.x += CHIP_SIZE;
		}
		chipPos.y += CHIP_SIZE;
	}

	//プレイヤーの描画位置計算
	{
		p->pos.x = p->m_pos.x - camera_pos.x + DRAW_CHIP_W * CHIP_SIZE / 2;
		p->pos.y = p->m_pos.y - camera_pos.y + DRAW_CHIP_H * CHIP_SIZE / 2;
	}
}