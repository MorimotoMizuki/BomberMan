//ゲームシーン
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "title.h"
#include"obj.h"
#include "function.h"

//プレイヤーのステータス
PlayerStatus gPlayerStatus = {
	2,		//life
	PLAYER_SPEED,	//speed
	1,		//bombPutNum
	1,		//bombLevel
	0,		//スコア
	false,	//リモコンフラグ
	false,	//爆弾通過フラグ
	false,	//壁通過フラグ
	false,	//火炎バリアフラグ
	false,	//パーフェクトマンフラグ
};
//現在の設置している爆弾の数
int gNowBombNum = 0;

//現在のマップ[y][x]
int gNowMap[MAP_CHIP_H + 1][MAP_CHIP_W + 1] = { 0 };

//ゲームの状態
GamePhaseId gGamePhase{ GamePhaseId::IDLE };

//現在のステージ番号
int gNowStageNum{ 1 };

//敵を倒した数
int gKillEnemyNum{ 0 };

//敵の総数
int gEnemySum{ 0 };

//敵の描画順番
int gEnemyPri{ Pri_Id::pENEMY };

//爆弾のID設定用
int gBombId{0};

//爆弾を爆発させた個数
int gBombExplosionNum{ 0 };

//破壊可能ブロックの爆破フラグ
bool gIsExplosionBlock{ false };

//セーブデータを読み込むフラグ
bool gIsLoadSaveData = false;

//ハイスコア保存用
int gHighScore{ 0 };

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p)
{
	//サウンド読み込み
	BGM = LoadSoundMem("sound\\NormalBGM.wav");
	SE_AllEnemyKill = LoadSoundMem("sound\\AllEnemyKnockDown.wav");

	//敵の描画順番初期化
	gEnemyPri = Pri_Id::pENEMY;

	//敵の合計数初期化
	gEnemySum = 0;

	//マップマネージャー
	map = std::make_unique<CMap>();
	map->LoadMap();	//マップデータ読み込み
	map->Map_Obj_Creation(base);//マップ生成

	//敵の合計数を取得
	//gEnemySum = map->GetStageEnemyTotal();

	//プレイヤー生成
	base.emplace_back((unique_ptr<BaseVector>)new CPlayer());

	//プレイ状態
	gGamePhase = GamePhaseId::PLAING;

	//タイマー設定
	Time = 200 * 60;

	//BGMをループで再生
	My_PlaySoundMem(BGM, DX_PLAYTYPE_LOOP, TRUE, MusicVolume::BGM_Stage);
}

//更新処理
int CGame::Update()
{
	if (gGamePhase == GamePhaseId::GAMEOVER || 
		gGamePhase == GamePhaseId::GAMECLEAR)
	{
		//BGMが再生していた場合は BGM を停止させる
		if (CheckSoundMem(BGM))
			StopSoundMem(BGM);

		if(gGamePhase == GamePhaseId::GAMEOVER)
			WaitTimer(2300); //2.3秒
		else
			WaitTimer(4300); //4.3秒

		//シーンの削除
		manager->Scene_Delete();
		//タイトルシーンに移行 : シーンを作成
		manager->scene = new CTitle(manager);
		return 0;
	}

	//敵の総数と敵を倒した数が等しくなった場合
	if (gEnemySum == gKillEnemyNum && !IsGoalOpen)
	{
		CDoor* door = (CDoor*)Get_obj(base, GOAL);
		if(door != nullptr)
			door->IsOpen = true;

		IsGoalOpen = true;

		//SE再生
		My_PlaySoundMem(SE_AllEnemyKill, DX_PLAYTYPE_BACK, TRUE, MusicVolume::SE_AllEnemyKill);
	}
	if(gEnemySum != gKillEnemyNum)
		IsGoalOpen = false;

	//更新処理
	for (int i = 0; i < base.size(); i++)
		base[i]->Action(base);

	//削除処理
	for (auto i = base.begin(); i != base.end();)
		(*i)->FLAG ? i++ : i = base.erase(i);

	//オブジェクトのソート処理(クイックソート)
	ObjSort_Quick(base, 0, base.size() - 1);

	if (Time == 0 && !IsTimeOver)
	{
		IsTimeOver = true;

		//敵を全て削除
		for (int i = 0; i < base.size(); i++)
		{
			if (base[i].get()->ID == Obj_Id::ENEMY) {
				((CBaseEnemy*)base[i].get())->EnemyInstantDead();
			}
		}

		//ポンタン生成
		for (int i = 0; i < 5; i++) {

			//ランダムで生成する座標設定
			MapPoint s_p{ 0,0 };
			s_p.x = Range_Random_Number(1, MAP_CHIP_W - 1);
			s_p.y = Range_Random_Number(1, MAP_CHIP_H - 1);

			//空白ではない場合はコンテニュー
			if (gNowMap[s_p.y][s_p.x] != -1) continue;

			//システム座標から座標を計算
			Point p{ s_p.x * CHIP_SIZE, s_p.y * CHIP_SIZE + WINDOW_HEADER };

			//ポンタン生成
			base.emplace_back((unique_ptr<BaseVector>) new CPontan(p, s_p));
		}
	}
	else
		Time--; //タイマー

	return 0;
}

//描画処理
void CGame::Draw()
{
	//ヘッダーの背景
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEADER, GetColor(173, 173, 173), 1);
	//ゲーム背景
	DrawBox(0, WINDOW_HEADER, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(56, 135, 0), 1);

	//ライフ
	Point lifePos{ static_cast<float>(WINDOW_WIDTH / 2) + 300.0f , 45.0f };
	float distance = 2.0f;
	DrawExtendFormatString(lifePos.x + distance, lifePos.y + distance, 2.0f, 2.0f, GetColor(0, 0, 0), "LIFE  %d", gPlayerStatus.life);
	DrawExtendFormatString(lifePos.x, lifePos.y, 2.0f, 2.0f, GetColor(255, 255, 255), "LIFE  %d", gPlayerStatus.life);

	//スコア
	Point scorePos{ lifePos.x - 300.0f, lifePos.y };
	DrawExtendFormatString(scorePos.x + distance, scorePos.y + distance, 2.0f, 2.0f, GetColor(0, 0, 0), "%d", gPlayerStatus.score);
	DrawExtendFormatString(scorePos.x, scorePos.y, 2.0f, 2.0f, GetColor(255, 255, 255), "%d", gPlayerStatus.score);

	//タイマー
	if (Time >= 0) {
		Point timePos{ 0.0f + 50.0f, scorePos.y };
		DrawExtendFormatString(timePos.x + distance, timePos.y + distance, 2.0f, 2.0f, GetColor(0, 0, 0), "TIME %d", Time / 60);
		DrawExtendFormatString(timePos.x, timePos.y, 2.0f, 2.0f, GetColor(255, 255, 255), "TIME %d", Time / 60);
	}

	//vectorオブジェクトの描画
	for (int i = 0; i < base.size(); i++)
		if(base[i]->FLAG) base[i]->Draw();
}

CGame::~CGame()
{
	DeleteSoundMem(BGM);
	DeleteSoundMem(SE_AllEnemyKill);
}
