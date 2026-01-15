//ゲームシーン
#include "DxLib.h"
#include "Scene_Manager.h"
#include "title.h"
#include "game.h"
#include"obj.h"
#include "function.h"

//ステータスデータ読み込み
void CTitle::LoadStatus()
{
	ifstream fp("text_data\\player_status.csv");
	string str; //読み込んだ文字列

	if (!fp.fail())
	{
		for (int y = 0; y < 11; y++)
		{
			getline(fp, str);//1行読み込み
			vector<string> strv = split(str, ',');//カンマで分割

			switch (y)
			{
			case LIFE:
				gPlayerStatus.life = stoi(strv.at(0));//文字列を数値に変換して保存
				break;
			case SPEED:
				gPlayerStatus.speed = stof(strv.at(0));//文字列を数値に変換して保存
				break;
			case BOMB_PUT_NUM:
				gPlayerStatus.bombPutNum = stoi(strv.at(0));//文字列を数値に変換して保存
				break;
			case BOMB_LEVEL:
				gPlayerStatus.bombLevel = stoi(strv.at(0));//文字列を数値に変換して保存
				break;
			case SCORE:
				gPlayerStatus.score = stoi(strv.at(0));//文字列を数値に変換して保存
				break;
			case REMOTECONTROLLER:
				gPlayerStatus.isRemoteController = stoi(strv.at(0)) == 0 ? false : true;
				break;
			case BOMB_PASS:
				gPlayerStatus.isBombPass = stoi(strv.at(0)) == 0 ? false : true;
				break;
			case WALL_PASS:
				gPlayerStatus.isWallPass = stoi(strv.at(0)) == 0 ? false : true;
				break;
			case FLAMEBARRIER:
				gPlayerStatus.isFlameBarrier = stoi(strv.at(0)) == 0 ? false : true;
				break;
			case PERFECTMAN:
				gPlayerStatus.isPerfectMan = stoi(strv.at(0)) == 0 ? false : true;
				break;
			case STAGE_NUM:
				gNowStageNum = stoi(strv.at(0));//文字列を数値に変換して保存
				break;
			}
		}

		fp.close();//ファイルを閉じる

		gIsLoadSaveData = true;
	}
}

//コンストラクタ
CTitle::CTitle(CManager* p) :CScene(p)
{
	//サウンド読み込み
	BGM_Normal = LoadSoundMem("sound\\Title.wav");
	BGM_GameOver = LoadSoundMem("sound\\GameOver.wav");
	SE_StageStart = LoadSoundMem("sound\\StageStart.wav");

	//画像読み込み
	TitleRogo_img = LoadGraph("image\\title_rogo.png");

	//セーブデータ読み込み
	if (gIsLoadSaveData == false)
		LoadStatus();

	//ハイスコア設定
	if (gPlayerStatus.score > gHighScore)
		gHighScore = gPlayerStatus.score;

	ResetPlayerStatus(); //ステータスをリセット

	//ゲームオーバーの場合
	if (gGamePhase == GamePhaseId::GAMEOVER)
	{
		if (gPlayerStatus.life == 0)
		{
			ScreenPhase = ScreenPhaseId::GAMEOVER_screen; //ゲームオーバースクリーンにする
			gPlayerStatus.life = 2; //(仮) ライフ 2 設定
			gPlayerStatus.score = 0;//スコア

			//ゲームオーバーの BGM 再生
			My_PlaySoundMem(BGM_GameOver, DX_PLAYTYPE_BACK, TRUE, MusicVolume::BGM_GameOver);
		}
		else
		{
			gPlayerStatus.life -= 1; //ライフ -1
			ScreenPhase = ScreenPhaseId::STAGE_TO_screen;
		}
		gGamePhase = GamePhaseId::IDLE;	//待機状態
	}
	//ゲームクリアの場合
	else if (gGamePhase == GamePhaseId::GAMECLEAR)
	{
		if(gNowStageNum < STAGE_SUM)
			gNowStageNum++;
		ScreenPhase = ScreenPhaseId::STAGE_TO_screen;
	}
	else
	{
		//通常BGMをループで再生
		My_PlaySoundMem(BGM_Normal, DX_PLAYTYPE_LOOP, TRUE, MusicVolume::BGM_Title);
	}
}

//更新処理
int CTitle::Update()
{
	//スペースキー入力
	if (Key_Check(Move_Id::SPACE) && !PushKey[KeyId::SPACE_key])
	{
		switch (TitleComId)
		{
		case TitleCommandId::START_COM:
			TrianglePos.y += MOVE_WIDTH;				//移動
			TitleComId = TitleCommandId::CONTINUE_COM;	//コンテニューに設定
			break;
		case TitleCommandId::CONTINUE_COM:
			TrianglePos = START_TRIANGLE_POS;			//初期座標
			TitleComId = TitleCommandId::START_COM;		//スタートに設定
			break;
		}
	}
	//エンターキー入力
	if (Key_Check(Move_Id::ENTER) && !PushKey[KeyId::ENTER_key])
	{
		switch (ScreenPhase)
		{
		case ScreenPhaseId::TITLE_screen: {
			switch (TitleComId)
			{
			case TitleCommandId::START_COM:
				//ステージ移動状態にする
				ScreenPhase = ScreenPhaseId::STAGE_TO_screen;
				//BGM が再生中なら停止
				if (CheckSoundMem(BGM_Normal)) {
					StopSoundMem(BGM_Normal);
				}
				return 0;
				break;
			case TitleCommandId::CONTINUE_COM:
				break;
			}
			break;
		}
		case ScreenPhaseId::GAMEOVER_screen: {
			ScreenPhase = ScreenPhaseId::TITLE_screen;
			if (CheckSoundMem(BGM_GameOver)) {
				StopSoundMem(BGM_GameOver); //ゲームオーバーBGMを停止
			}
			My_PlaySoundMem(BGM_Normal, DX_PLAYTYPE_LOOP, TRUE, MusicVolume::BGM_Title); //通常BGMをループ再生
			break;
		}
		}
	}

	//ステージ移動状態の場合
	if (ScreenPhase == ScreenPhaseId::STAGE_TO_screen) {

		if (TimerCnt >= 3 * 60)
		{
			//シーンの削除
			manager->Scene_Delete();
			//ゲームシーンに移行 : シーンを作成
			manager->scene = new CGame(manager);
			return 0;
		}
		else {
			TimerCnt++;
			if (!IsStageStartSE) {
				My_PlaySoundMem(SE_StageStart, DX_PLAYTYPE_BACK, TRUE, MusicVolume::SE_StageStart);//SE 再生
				IsStageStartSE = true;
			}
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

	//キー更新
	PushKey[KeyId::SPACE_key] = Key_Check(Move_Id::SPACE);
	PushKey[KeyId::ENTER_key] = Key_Check(Move_Id::ENTER);

	return 0;
}

//描画処理
void CTitle::Draw()
{
	switch (ScreenPhase)
	{
		case ScreenPhaseId::TITLE_screen:
		{
			//タイトル画像描画
			DrawExtendGraph(64 * 1, 64 * 1,64 * 1 + 64 * 14, 64 * 1 + 64 * 8, TitleRogo_img, false);
			Point startPos{ START_TRIANGLE_POS.x + 95.0f, START_TRIANGLE_POS.y };
			//スタート描画
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "START");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "START");
			//コンテニュー描画
			startPos = { startPos.x, startPos.y + MOVE_WIDTH };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "CONTINUE");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "CONTINUE");
			//三角形描画
			DrawTriangle(TrianglePos.x + DISTANCE, TrianglePos.y + DISTANCE, TrianglePos.x + DISTANCE, TrianglePos.y + TRIANGLE_SIZE + DISTANCE, TrianglePos.x + TRIANGLE_SIZE + DISTANCE, TrianglePos.y + TRIANGLE_SIZE / 2.0f + DISTANCE, GetColor(128, 128, 128), true);
			DrawTriangle(TrianglePos.x, TrianglePos.y, TrianglePos.x, TrianglePos.y + TRIANGLE_SIZE, TrianglePos.x + TRIANGLE_SIZE, TrianglePos.y + TRIANGLE_SIZE / 2.0f, GetColor(255, 255, 255), true);
			//TOPを描画
			startPos = { startPos.x - 145, startPos.y + 100 };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "TOP");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "TOP");
			//スコアを右詰めで描画
			startPos.x += 365;
			//スコアが 0 の場合のみ 00 と描画、それ以外はスコアを描画
			std::string str = (gHighScore == 0) ? "00" : std::to_string(gHighScore);
			//表示幅を取得
			float w = static_cast<float>(GetDrawStringWidth(str.c_str(), str.size())) * STRING_EXTEND_X;
			DrawExtendFormatString(startPos.x + DISTANCE - w, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "%s", str.c_str());
			DrawExtendFormatString(startPos.x - w, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "%s", str.c_str());
			//下に書いてある文字
			startPos = { startPos.x - 500.0f, startPos.y + 60.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "COPYRIGHT 1985 HUDSON SOFT");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "COPYRIGHT 1985 HUDSON SOFT");
			break;
		}
		case ScreenPhaseId::GAMEOVER_screen:
		{
			Point startPos{ WINDOW_WIDTH / 2 - 120.0f, WINDOW_HEIGHT / 2 - 50.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "GAMEOVER");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "GAMEOVER");
			break;
		}
		case ScreenPhaseId::STAGE_TO_screen:
		{
			Point startPos{ WINDOW_WIDTH / 2 - 120.0f, WINDOW_HEIGHT / 2 - 50.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "STAGE%d", gNowStageNum);
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "STAGE%d", gNowStageNum);
			break;
		}
	}

	for (int i = 0; i < base.size(); i++)
		if (base[i]->FLAG) base[i]->Draw();
}

CTitle::~CTitle()
{
	DeleteSoundMem(BGM_Normal);
	DeleteSoundMem(BGM_GameOver);
	DeleteSoundMem(SE_StageStart);

	DeleteGraph(TitleRogo_img);
}
