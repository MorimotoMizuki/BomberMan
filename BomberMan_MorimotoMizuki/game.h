//ゲームシーンヘッダ
#pragma once
#include "objBase.h"
#include "CCamera.h"
#include"map.h"

class CGame :public CScene
{
private:
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
	//SE 全ての敵を倒した用
	int SE_AllEnemyKill{ -1 };

	//タブキー入力制限用
	bool TabKeyCheck{ false };

	//ゲームフェーズ保存用
	GamePhaseId PrevGamePhase{ GamePhaseId::IDLE };
};