//ゲームシーン
#include "DxLib.h"
#include "Scene_Manager.h"
#include "title.h"
#include "game.h"
#include"obj.h"
#include "function.h"

//コンストラクタ
CTitle::CTitle(CManager* p) :CScene(p)
{
	//ゲームオーバーの場合
	if (gGamePhase == GamePhaseId::GAMEOVER)
	{
		if (gPlayerStatus.life == 0)
		{
			ScreenPhase = ScreenPhaseId::GAMEOVER_screen; //ゲームオーバースクリーンにする
			gPlayerStatus.life = 2; //(仮) ライフ 2 設定
		}
		else
		{
			gPlayerStatus.life -= 1; //ライフ -1
			ScreenPhase = ScreenPhaseId::STAGE_TO_screen;
		}
		gGamePhase = GamePhaseId::IDLE;	//待機状態

		//プレイヤーのステータスを初期化
		gPlayerStatus.bombLevel = 1;
		gPlayerStatus.bombPutNum = 1;
		gPlayerStatus.speed = PLAYER_SPEED;

	}
	//ゲームクリアの場合
	else if (gGamePhase == GamePhaseId::GAMECLEAR)
	{
		if(gNowStageNum < STAGE_SUM)
			gNowStageNum++;
		ScreenPhase = ScreenPhaseId::STAGE_TO_screen;
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
				//シーンの削除
				manager->Scene_Delete();
				//ゲームシーンに移行 : シーンを作成
				manager->scene = new CGame(manager);
				return 0;
				break;
			case TitleCommandId::CONTINUE_COM:
				break;
			}
			break;
		}
		case ScreenPhaseId::GAMEOVER_screen: {
			ScreenPhase = ScreenPhaseId::TITLE_screen;
			break;
		}
		}
	}

	//ステージ移動状態の場合
	if (ScreenPhase == ScreenPhaseId::STAGE_TO_screen) {

		if (TimerCnt >= 120)
		{
			//シーンの削除
			manager->Scene_Delete();
			//ゲームシーンに移行 : シーンを作成
			manager->scene = new CGame(manager);
			return 0;
		}
		else
			TimerCnt++;
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
		//仮タイトル
		DrawExtendFormatString(0.0f, 0.0f, 3.0f, 3.0f, GetColor(255, 255, 0), "BOMBERMAN");
		Point startPos{ START_TRIANGLE_POS.x + 50.0f, START_TRIANGLE_POS.y };
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

}
