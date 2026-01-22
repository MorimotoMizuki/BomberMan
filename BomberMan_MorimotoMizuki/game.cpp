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

//最強のプレイヤーのステータス
PlayerStatus gSuperPlayerStatus = {
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

//ボーナスステージフラグ
bool gIsBonusStage{ false };

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

	if (gIsBonusStage) {
		SE_StageClear = LoadSoundMem("sound\\StageClear.wav");
		BGM_BonusStage = LoadSoundMem("sound\\BonusStage.wav");
	}

	//敵の描画順番初期化
	gEnemyPri = Pri_Id::pENEMY;

	//敵の合計数初期化
	gEnemySum = 0;

	//マップマネージャー
	map = std::make_unique<CMap>();
	map->LoadMap();	//マップデータ読み込み
	map->Map_Obj_Creation(base);//マップ生成

	//プレイヤー生成
	base.emplace_back((unique_ptr<BaseVector>)new CPlayer());

	//プレイ状態
	gGamePhase = GamePhaseId::PLAING;

	//タイマー設定
	if (gIsBonusStage)		//ボーナスステージの場合は 30秒
		Time = BONUS_TIME;
	else
		Time = NORMAL_TIME; //通常ステージの場合は 200秒

	//BGMをループで再生
	if(gIsBonusStage)
		My_PlaySoundMem(BGM_BonusStage, DX_PLAYTYPE_LOOP, TRUE, MusicVolume::BGM_Bonus);
	else
		My_PlaySoundMem(BGM, DX_PLAYTYPE_LOOP, TRUE, MusicVolume::BGM_Stage);
}

//更新処理
int CGame::Update()
{
	switch (gGamePhase)
	{
	case PLAING:
	{
		if (gIsBonusStage)
			BonusStagePopEnemy(); //ボーナスステージ時の敵の無限出現処理

		//タブキー処理
		if (TabKey_Action(GamePhaseId::PLAING)) return 0;

		if (!gIsBonusStage) {
			//敵の総数と敵を倒した数が等しくなった場合
			if (gEnemySum == gKillEnemyNum && !IsGoalOpen)
			{
				CDoor* door = (CDoor*)Get_obj(base, GOAL);
				if (door != nullptr) door->IsOpen = true;

				IsGoalOpen = true;

				//SE再生
				My_PlaySoundMem(SE_AllEnemyKill, DX_PLAYTYPE_BACK, TRUE, MusicVolume::SE_AllEnemyKill);
			}
		}

		//制限時間処理
		TimerAction();
		break;
	}
	case GAMEOVER:
	case GAMECLEAR:
	{
		//BGMが再生していた場合は BGM を停止させる
		if (CheckSoundMem(BGM))
			StopSoundMem(BGM);

		if (gGamePhase == GamePhaseId::GAMEOVER)
			WaitTimer(2300); //2.3秒
		else
			WaitTimer(4300); //4.3秒

		//シーンの削除
		manager->Scene_Delete();
		//タイトルシーンに移行 : シーンを作成
		manager->scene = new CTitle(manager);
		return 0;
	}
	case PAUSE:
	{
		//タブキー処理
		if (TabKey_Action(GamePhaseId::PLAING)) return 0;
		//Tキー処理
		if (Key_Check(Move_Id::T_KEY))
		{
			//BGMが再生していた場合は BGM を停止させる
			if (CheckSoundMem(BGM))
				StopSoundMem(BGM);

			//シーンの削除
			manager->Scene_Delete();
			//タイトルシーンに移行 : シーンを作成
			manager->scene = new CTitle(manager);
			return 0;
		}

		//プレイヤーを取得
		CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
		if (p != nullptr) p->StopPlayerSound();

		return 0;
	}
	}

	//更新処理
	for (int i = 0; i < base.size(); i++)
		base[i]->Action(base);

	//削除処理
	for (auto i = base.begin(); i != base.end();)
		(*i)->FLAG ? i++ : i = base.erase(i);

	//オブジェクトのソート処理(クイックソート)
	ObjSort_Quick(base, 0, base.size() - 1);

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

	//ポーズ画面
	if (gGamePhase == GamePhaseId::PAUSE) {

		//タイトル画面への遷移方法描画
		DrawExtendFormatString(0 + distance, 0 + distance, 2.0f, 2.0f, GetColor(0, 0, 0), "T_Key : Title");
		DrawExtendFormatString(0, 0, 2.0f, 2.0f, GetColor(255, 255, 255), "T_Key : Title");
	}

	//vectorオブジェクトの描画
	for (int i = 0; i < base.size(); i++)
		if(base[i]->FLAG) base[i]->Draw();
}

CGame::~CGame()
{
	DeleteSoundMem(BGM);
	DeleteSoundMem(SE_AllEnemyKill);

	if (gIsBonusStage) {
		DeleteSoundMem(SE_StageClear);
		DeleteSoundMem(BGM_BonusStage);
	}
}

//ボーナスステージの敵の出現処理関数
void CGame::BonusStagePopEnemy()
{
	if (!gIsBonusStage) return;

	//ボーナスステージの敵の最大出現数を超えていた場合は終了
	if ((gEnemySum - gKillEnemyNum) > 8) return;

	while (true)
	{
		//ランダムで生成する座標設定
		MapPoint s_p{ 0,0 };
		s_p.x = Range_Random_Number(1, MAP_CHIP_W - 1);
		s_p.y = Range_Random_Number(1, MAP_CHIP_H - 1);

		//通常ブロックの場合はコンテニュー
		if (gNowMap[s_p.y][s_p.x] == Obj_Id::BLOCK) continue;

		//システム座標から座標を計算
		Point p{ s_p.x * CHIP_SIZE, s_p.y * CHIP_SIZE + WINDOW_HEADER };

		int enemy_pop_stage_num = 0;

		for (int i = 0; i < BONUS_STAGE_NUM.size(); i++) {
			if (gNowStageNum - 1 == BONUS_STAGE_NUM[i])
				enemy_pop_stage_num = i;
		}

		//敵生成
		switch (enemy_pop_stage_num)
		{
		case 0:
			base.emplace_back((unique_ptr<BaseVector>) new CBallom(p, s_p));
			break;
		case 1:
			base.emplace_back((unique_ptr<BaseVector>) new COneal(p, s_p));
			break;
		case 2:
			base.emplace_back((unique_ptr<BaseVector>) new CDahl(p, s_p));
			break;
		case 3:
			base.emplace_back((unique_ptr<BaseVector>) new CMinvo(p, s_p));
			break;
		case 4:
			base.emplace_back((unique_ptr<BaseVector>) new CKondoria(p, s_p));
			break;
		case 5:
			base.emplace_back((unique_ptr<BaseVector>) new COvapee(p, s_p));
			break;
		case 6:
			base.emplace_back((unique_ptr<BaseVector>) new CPass(p, s_p));
			break;
		case 7:
		case 8:
			base.emplace_back((unique_ptr<BaseVector>) new CPontan(p, s_p));
			break;
		}
		return;
	}
}

//制限時間処理関数
void CGame::TimerAction()
{
	//制限時間オーバー
	if (Time == 0)
	{
		if (IsTimeOver) return;

		IsTimeOver = true;

		if (gIsBonusStage) {

			//BGMが再生していた場合は BGM を停止させる
			if (CheckSoundMem(BGM_BonusStage))
				StopSoundMem(BGM_BonusStage);
			//SE再生
			My_PlaySoundMem(SE_StageClear, DX_PLAYTYPE_BACK, TRUE, MusicVolume::SE_StageClear);
			gGamePhase = GamePhaseId::GAMECLEAR;
		}
		else {
			//敵を全て削除
			for (int i = 0; i < base.size(); i++)
			{
				if (base[i].get()->ID == Obj_Id::ENEMY) {
					((CBaseEnemy*)base[i].get())->EnemyInstantDead();
				}
			}
			int pop_pontan{ 0 };
			//ポンタン生成
			while (pop_pontan < 10)
			{
				//ランダムで生成する座標設定
				MapPoint s_p{ 0,0 };
				s_p.x = Range_Random_Number(1, MAP_CHIP_W - 1);
				s_p.y = Range_Random_Number(1, MAP_CHIP_H - 1);

				//通常ブロックの場合はコンテニュー
				if (gNowMap[s_p.y][s_p.x] == Obj_Id::BLOCK) continue;

				//システム座標から座標を計算
				Point p{ s_p.x * CHIP_SIZE, s_p.y * CHIP_SIZE + WINDOW_HEADER };

				//ポンタン生成
				base.emplace_back((unique_ptr<BaseVector>) new CPontan(p, s_p));
				pop_pontan++;
			}
		}
	}
	else
		Time--; //タイマー
}

//タブキーの処理関数 : Pauseじゃない場合のゲーム状態
bool CGame::TabKey_Action(GamePhaseId change_game_phase)
{
	//タブキー入力
	if (Key_Check(Move_Id::TAB_KEY) && !TabKeyCheck)
	{
		TabKeyCheck = true;

		if (gGamePhase == GamePhaseId::PAUSE)
			gGamePhase = change_game_phase;
		else
			gGamePhase = GamePhaseId::PAUSE;

		return true;
	}

	//タブキー更新
	TabKeyCheck = Key_Check(Move_Id::TAB_KEY);
	return false;
}
