//ゲームシーンヘッダ
#pragma once
#include "objBase.h"
#include "CCamera.h"
#include"map.h"
#include<array>

class CGame :public CScene
{
private:

	//通常ステージとボーナスステージの制限時間フレーム
	static constexpr auto NORMAL_TIME = 200 * 60;
	static constexpr auto BONUS_TIME  = 30 * 60;

public:
	//オブジェクト
	//リスト
	//list<unique_ptr<Base3D>> base3D;//3Dオブジェクト
	//list<unique_ptr<Base>> base;//2Dオブジェクト
	//ベクター
	vector<unique_ptr<BaseVector>> base;

	//コンストラクタ
	CGame(CManager*);

	//デストラクタ
	~CGame();

	int Update();//更新処理
	void Draw();//描画処理

	//ボーナスステージの敵の出現処理関数
	void BonusStagePopEnemy();

	//制限時間処理関数
	void TimerAction();

	//タブキーの処理関数: Pauseじゃない場合のゲーム状態
	bool TabKey_Action(GamePhaseId change_game_phase);

	//マップManager
	std::unique_ptr<CMap> map;

	//ゴール開閉フラグ
	bool IsGoalOpen{ false };

	//タイマー
	int Time{ 0 };
	bool IsTimeOver{ false };

	//CCamera* camera;//カメラオブジェクト

	//BGMサウンド用
	int BGM{ -1 };
	//ボーナスステージBGM
	int BGM_BonusStage{ -1 };
	//SE 全ての敵を倒した用
	int SE_AllEnemyKill{ -1 };
	//ゲームクリア時
	int SE_StageClear{ -1 };

	//タブキー入力制限用
	bool TabKeyCheck{ false };
};