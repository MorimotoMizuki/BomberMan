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

//最強ステータスデータ読み込み
void CTitle::LoadSuperStatus()
{
	ifstream fp("text_data\\super_player_status.csv");
	string str; //読み込んだ文字列

	if (!fp.fail())
	{
		for (int y = 0; y < 10; y++)
		{
			getline(fp, str);//1行読み込み
			vector<string> strv = split(str, ',');//カンマで分割

			switch (y)
			{
			case LIFE:
				gSuperPlayerStatus.life = stoi(strv.at(0));//文字列を数値に変換して保存
				break;
			case SPEED:
				gSuperPlayerStatus.speed = stof(strv.at(0));//文字列を数値に変換して保存
				break;
			case BOMB_PUT_NUM:
				gSuperPlayerStatus.bombPutNum = stoi(strv.at(0));//文字列を数値に変換して保存
				break;
			case BOMB_LEVEL:
				gSuperPlayerStatus.bombLevel = stoi(strv.at(0));//文字列を数値に変換して保存
				break;
			case REMOTECONTROLLER:
				gSuperPlayerStatus.isRemoteController = stoi(strv.at(0)) == 0 ? false : true;
				break;
			case BOMB_PASS:
				gSuperPlayerStatus.isBombPass = stoi(strv.at(0)) == 0 ? false : true;
				break;
			case WALL_PASS:
				gSuperPlayerStatus.isWallPass = stoi(strv.at(0)) == 0 ? false : true;
				break;
			case FLAMEBARRIER:
				gSuperPlayerStatus.isFlameBarrier = stoi(strv.at(0)) == 0 ? false : true;
				break;
			case PERFECTMAN:
				gSuperPlayerStatus.isPerfectMan = stoi(strv.at(0)) == 0 ? false : true;
				break;
			}
		}

		fp.close();//ファイルを閉じる
	}
}

//コンストラクタ
CTitle::CTitle(CManager* p) :CScene(p)
{
	//サウンド読み込み
	BGM_Normal = LoadSoundMem("sound\\Title.wav");
	BGM_GameOver = LoadSoundMem("sound\\GameOver.wav");
	BGM_Ending = LoadSoundMem("sound\\FiftyStageClear.wav");
	SE_StageStart = LoadSoundMem("sound\\StageStart.wav");
	SE_PlayerWalk = LoadSoundMem("sound\\Walk_Width.wav");

	//画像読み込み
	TitleRogo_img = LoadGraph("image\\title_rogo.png");
	Renga_img = LoadGraph("image\\renga.png");

	LoadDivGraph("image\\ending_player.png", 8, 4, 2, IMGSIZE16, IMGSIZE16, EndingPlayer_img);

	//char配列の初期化
	std::fill(std::begin(KeyState), std::end(KeyState), 0);
	std::fill(std::begin(PrevKeyState), std::end(PrevKeyState), 0);

	//セーブデータ読み込み
	if (gIsLoadSaveData == false) {
		LoadStatus();
		LoadSuperStatus();
	}

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
		if (gNowStageNum < STAGE_SUM) {

			auto it = std::find(BONUS_STAGE_NUM.begin(), BONUS_STAGE_NUM.end(), gNowStageNum);
			//ボーナスステージの場合
			if (it != BONUS_STAGE_NUM.end())
				gIsBonusStage = true;

			auto next = std::find(BONUS_STAGE_NUM.begin(), BONUS_STAGE_NUM.end(), gNowStageNum - 1);
			if (next != BONUS_STAGE_NUM.end() && gIsBonusStage)
				gIsBonusStage = false;
			else
				gNowStageNum++;

			ScreenPhase = ScreenPhaseId::STAGE_TO_screen;
		}
		//最終ステージをクリアした場合はエンディング
		else if (gNowStageNum == STAGE_SUM) {
			gNowStageNum = 1; //ステージは1から
			gIsBonusStage = false;
			ScreenPhase = ScreenPhaseId::ENDING_screen; //エンディングのスクリーンへ

			//エンディング用のプレイヤーの座標
			EndingPlayerPos = { 0 * CHIP_SIZE, WINDOW_HEIGHT - 150 - CHIP_SIZE };
			AnimMaxIndex = 3;

			//エンディングBGMを単発で再生
			My_PlaySoundMem(BGM_Ending, DX_PLAYTYPE_BACK, TRUE, MusicVolume::BGM_Ending);
		}

		gPlayerStatus.life++; //ライフを増やす
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
	SpaceKey_Action();
	//エンターキー入力
	EnterKey_Action();

	//各スクリーンの更新処理
	switch (ScreenPhase)
	{
	//ステージ移動用スクリーン
	case ScreenPhaseId::STAGE_TO_screen:
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
		break;
	//コンテニュー用スクリーン
	case ScreenPhaseId::CONTINUE_screen:

		if (IsPassword) {
			//ステージ移動状態にする
			ScreenPhase = ScreenPhaseId::STAGE_TO_screen;
			//BGM が再生中なら停止
			if (CheckSoundMem(BGM_Normal)) {
				StopSoundMem(BGM_Normal);
			}
		}

		UpdateKeyState();		//キーボード入力の更新
		UpdatePasswordInput();	//キーボード入力

		//点滅処理
		FlashCnt++;
		//一定フレーム後に点滅フラグ反転
		if (FlashCnt > 30) {
			IsFlashVisible = !IsFlashVisible;
			FlashCnt = 0;
		}

		break;
	//エンディング用スクリーン
	case ScreenPhaseId::ENDING_screen:

		EndingAction();
		break;
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
			//文字列描画
			Point startPos{ WINDOW_WIDTH / 2 - 120.0f, WINDOW_HEIGHT / 2 - 50.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "GAMEOVER");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "GAMEOVER");

			startPos = { startPos.x - 110.0f, startPos.y + 400.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "MNJDIJFHICNFLCBFEFEG");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "MNJDIJFHICNFLCBFEFEG");
			break;
		}
		case ScreenPhaseId::STAGE_TO_screen:
		{
			//文字列描画
			Point startPos{ WINDOW_WIDTH / 2 - 120.0f, WINDOW_HEIGHT / 2 - 50.0f };
			if (gIsBonusStage) {
				startPos.x = WINDOW_WIDTH / 2 - 150.0f;
				DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "BONUS STAGE");
				DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "BONUS STAGE");
			}
			else {
				DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "STAGE%d", gNowStageNum);
				DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "STAGE%d", gNowStageNum);
			}
			break;
		}
		case ScreenPhaseId::CONTINUE_screen:
		{
			//文字列描画
			Point startPos{ WINDOW_WIDTH / 2 - 230.0f, WINDOW_HEIGHT / 2 - 300.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "ENTER SECRET CODE");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "ENTER SECRET CODE");
			startPos = { startPos.x, startPos.y + 200.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), Password.c_str());
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), Password.c_str());

			//点滅フラグtrue　と　パスワードが空白ではない場合
			if (IsFlashVisible && !Password.empty()) {
				//文字列全体の幅
				float total_w = static_cast<float>(GetDrawStringWidth(Password.c_str(), Password.size())) * STRING_EXTEND_X;
				//最後の1文字を取り出す
				char last_char[2] = { Password.back(), '\0' };
				//最後の1文字の幅
				float char_w  = static_cast<float>(GetDrawStringWidth(last_char, 1)) * STRING_EXTEND_X;
				//四角描画
				DrawBox(startPos.x + total_w - char_w, startPos.y, startPos.x + total_w - char_w + char_w, startPos.y + 30, GetColor(255, 255, 255), true);
			}

			break;
		}
		case ScreenPhaseId::ENDING_screen:
		{
			//文字列描画
			Point startPos{ WINDOW_WIDTH / 2 - 230.0f, WINDOW_HEIGHT / 2 - 300.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "CONGRATULATIONS");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "CONGRATULATIONS");
			startPos = { startPos.x - 135.0f, startPos.y + 50.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "BOMBER MAN BECOMES RUNNER");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "BOMBER MAN BECOMES RUNNER");
			startPos = { startPos.x - 28.0f, startPos.y + 50.0f };
			DrawExtendFormatString(startPos.x + DISTANCE, startPos.y + DISTANCE, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(128, 128, 128), "SEE YOU AGAIN IN LODE RUNNER");
			DrawExtendFormatString(startPos.x, startPos.y, STRING_EXTEND_X, STRING_EXTEND_Y, GetColor(255, 255, 255), "SEE YOU AGAIN IN LODE RUNNER");

			//レンガ描画
			for (int i = 0; i < 16; i++)
				DrawExtendGraph(i * CHIP_SIZE, EndingPlayerPos.y + CHIP_SIZE, i * CHIP_SIZE + CHIP_SIZE, EndingPlayerPos.y + CHIP_SIZE + CHIP_SIZE, Renga_img, false);

			//プレイヤー描画
			DrawExtendGraph(EndingPlayerPos.x, EndingPlayerPos.y, EndingPlayerPos.x + CHIP_SIZE, EndingPlayerPos.y + CHIP_SIZE, EndingPlayer_img[AnimIndex], true);
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
	DeleteSoundMem(BGM_Ending);
	DeleteSoundMem(SE_StageStart);

	DeleteGraph(TitleRogo_img);
}

//パスワード入力
void CTitle::UpdatePasswordInput()
{
	int ch;

	//入力された文字を全て取得
	while ((ch = GetInputChar(TRUE)) != 0)
	{
		//小文字 a～p → A～P
		if (ch >= 'a' && ch <= 'p') {
			ch = ch - 'a' + 'A';
		}

		//BackSpase
		if (IsKeyTrigger(KEY_INPUT_BACK))
		{
			if (!Password.empty())
				Password.pop_back();
		}
		//Enter : 確定
		if (IsKeyTrigger(KEY_INPUT_RETURN))
		{
			if (!Password.empty())
				IsPassword = IsValidPassword(Password); //パスワード検証
		}
		//A～P のみ受け付ける
		if ((ch >= 'A' && ch <= 'P') || 
			(ch >= '0' && ch <= '9'))
		{
			if (Password.size() < PASSWORD_MAX)
				Password.push_back((char)ch);

			IsFlashVisible = false;
			FlashCnt = 0;
		}
	}
}

//パスワード検証
bool CTitle::IsValidPassword(std::string& password)
{
	//文字数が違う場合はfalseで終了
	//if (password.size() != PASSWORD_MAX) return false;

	//文字が範囲内じゃない場合はfalseで終了
	//for (char c : password) {
	//	if (c < 'A' || c > 'P')
	//		return false;
	//}

	//数字かどうか判別
	if (Is_Int_from_Str(password, PasswordOutStageNum))
	{
		//ステージにある場合はそのステージにする
		if (gNowStageNum <= STAGE_SUM)
			gNowStageNum = PasswordOutStageNum;

		return true;
	}

	//パスワードと一致しているか判定
	for (const auto& p : MY_PASSWORD)
	{
		if (password == p.password)
		{
			switch (p.id)
			{
				case Password_Id::SAIKYOU:
				{
					SuperPlayerStatus();
					break;
				}
			}
			//文字をクリア
			Password.clear();
		}
	}

	return false;
}

//std::string が int に変換できるか判別 : できる場合は outBValueに変換
bool CTitle::Is_Int_from_Str(const std::string& password, int& outValue)
{
	//std::string が int に変換できる場合は変換
	auto result = std::from_chars(password.data(), password.data() + password.size(), outValue);

	//文字列全体が int に変換できたか判定
	return result.ec == std::errc{} && result.ptr == password.data() + password.size();
}

//毎フレーム更新 : キー
void CTitle::UpdateKeyState()
{
	memcpy(PrevKeyState, KeyState, 256);
	GetHitKeyStateAll(KeyState);
}

//押した瞬間判定 : キー
bool CTitle::IsKeyTrigger(int key)
{
	return KeyState[key] && !PrevKeyState[key];
}

//スペースキー処理関数
void CTitle::SpaceKey_Action()
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

		switch (ScreenPhase)
		{
			//コンテニュー画面
		case ScreenPhaseId::CONTINUE_screen:
			//タイトル画面にする
			ScreenPhase = ScreenPhaseId::TITLE_screen;
			break;
		}
	}

	//キー更新
	PushKey[KeyId::SPACE_key] = Key_Check(Move_Id::SPACE);
}

//エンターキー処理関数
void CTitle::EnterKey_Action()
{
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
				break;
			case TitleCommandId::CONTINUE_COM:
				//コンテニュー画面にする
				ScreenPhase = ScreenPhaseId::CONTINUE_screen;
				SetUseIMEFlag(FALSE); //IMEを無効化
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
		case ScreenPhaseId::CONTINUE_screen: {
			break;
		}
		}
	}

	//キー更新
	PushKey[KeyId::ENTER_key] = Key_Check(Move_Id::ENTER);
}

//エンディング処理
void CTitle::EndingAction()
{
	//トータルの移動量が一周半未満の場合はプレイヤー移動処理
	if (TotalMovePosX < WINDOW_WIDTH + WINDOW_WIDTH / 2 + CHIP_SIZE) {
		EndingPlayerPos.x += 2.0f;	//プレイヤー座標更新
		TotalMovePosX += 2.0f;		//トータルの移動量更新

		if (!CheckSoundMem(SE_PlayerWalk))
			My_PlaySoundMem(SE_PlayerWalk, DX_PLAYTYPE_BACK, TRUE, MusicVolume::SE_PlayerWalk_W - 32); //SE再生
	}
	//トータルの移動量が一周半を超えた場合は停止
	else {
		AnimIndex = AnimMaxIndex - 3;
		if (CheckSoundMem(SE_PlayerWalk)) //SE停止
			StopSoundMem(SE_PlayerWalk);

		//一定時間後にステージ移動画面に遷移
		if (EndingEndCnt > ENDING_TO_STAGE_FRAME)
			ScreenPhase = ScreenPhaseId::STAGE_TO_screen;
		else
			EndingEndCnt++;
		return;
	}

	//プレイヤーが範囲外の場合は左端からにする
	if (EndingPlayerPos.x > WINDOW_WIDTH)
		EndingPlayerPos.x = 0 - CHIP_SIZE;

	//ロードランナーに変身
	if (EndingPlayerPos.x > WINDOW_WIDTH / 2 - CHIP_SIZE && !IsRoadRunner) {

		IsRoadRunner = true;
		AnimCnt = 0;
		AnimIndex = 4;
		AnimMaxIndex = 7;
		return;
	}

	//アニメーションループ処理
	if (AnimCnt > 4) {
		AnimCnt = 0;

		if (AnimIndex < AnimMaxIndex)
			AnimIndex++;
		else
			AnimIndex = AnimMaxIndex - 3;
	}
	else
		AnimCnt++;
}