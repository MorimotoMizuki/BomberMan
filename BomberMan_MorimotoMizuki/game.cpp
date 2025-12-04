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
	6.0f,	//speed
	1,		//bombPutNum
	1,		//bombLevel
	0,		//スコア
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

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p)
{
	//マップマネージャー
	map = std::make_unique<CMap>();
	map->LoadMap();	//マップデータ読み込み
	map->Map_Obj_Creation(base);//マップ生成

	//敵の合計数を取得
	EnemySum = map->GetStageEnemyTotal(gNowStageNum);

	//プレイヤー生成
	base.emplace_back((unique_ptr<BaseVector>)new CPlayer());

	//プレイ状態
	gGamePhase = GamePhaseId::PLAING;

	//初期化
	gKillEnemyNum = 0; //倒した敵の数
	gPlayerStatus.score = 0; //スコア
}

//更新処理
int CGame::Update()
{
	//map->Action(base);

	if (gGamePhase == GamePhaseId::GAMEOVER || 
		gGamePhase == GamePhaseId::GAMECLEAR)
	{
		WaitTimer(1000);

		//シーンの削除
		manager->Scene_Delete();
		//タイトルシーンに移行 : シーンを作成
		manager->scene = new CTitle(manager);
		return 0;
	}

	if (EnemySum == gKillEnemyNum && !IsGoalOpen)
	{
		CDoor* door = (CDoor*)Get_obj(base, GOAL);
		if(door != nullptr)
			door->IsOpen = true;

		IsGoalOpen = true;
	}

	//更新処理
	for (int i = 0; i < base.size(); i++)
		base[i]->Action(base);

	//削除処理
	for (auto i = base.begin(); i != base.end();)
		(*i)->FLAG ? i++ : i = base.erase(i);

	//オブジェクトのソート処理(クイックソート)
	ObjSort_Quick(base, 0, base.size() - 1);

	//listオブジェクトの更新処理	
	//for (auto i = base.begin(); i != base.end(); i++)
	//	(*i)->Action(base);

	return 0;
}

//描画処理
void CGame::Draw()
{
	//オブジェクト個数
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Object_Count = %d", base.size());

	//ヘッダーの背景
	//DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEADER, GetColor(173, 173, 173), 1);
	//ゲーム背景
	DrawBox(0, WINDOW_HEADER, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(56, 135, 0), 1);

	DrawFormatString(0, 0, GetColor(255, 255, 255), "%d", gGamePhase);

	Point lifePos{ static_cast<float>(WINDOW_WIDTH / 2) + 150.0f , 45.0f };
	float distance = 2.0f;
	DrawExtendFormatString(lifePos.x + distance, lifePos.y + distance, 2.0f, 2.0f, GetColor(0, 0, 0), "LIFE : %d", gPlayerStatus.life);
	DrawExtendFormatString(lifePos.x, lifePos.y, 2.0f, 2.0f, GetColor(255, 255, 255), "LIFE : %d", gPlayerStatus.life);

	for (int i = 0; i < base.size(); i++)
		if(base[i]->FLAG) base[i]->Draw();

	//listオブジェクトの描画
	//for (auto i = base.begin(); i != base.end(); i++)
	//	if ((*i)->FLAG) (*i)->Draw();

	//3D軸の描画
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 0, 100),
	//	0x0000ff
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(100, 0, 0),
	//	0xff0000
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 100, 0),
	//	0x00ff00
	//);
}

CGame::~CGame()
{

}
