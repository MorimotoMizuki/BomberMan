//ゲームシーンヘッダ
#pragma once
#include "objBase.h"
#include "CCamera.h"
#include"map.h"

class CTitle :public CScene
{
private:

	//三角形の移動幅
	static constexpr auto MOVE_WIDTH = 60.0f;

	//三角形の初期描画座標
	static constexpr Point START_TRIANGLE_POS = { WINDOW_WIDTH / 2 - 190.0f, WINDOW_HEIGHT / 2 + 150.0f };

	//三角形のサイズ
	static constexpr auto TRIANGLE_SIZE = 30.0f;

	//文字の拡大率
	static constexpr auto STRING_EXTEND_X = 3.0f;
	static constexpr auto STRING_EXTEND_Y = 2.0f;

	//文字のずれ
	static constexpr auto DISTANCE = 2.0f;

public:

	enum TitleCommandId {
		START_COM,
		CONTINUE_COM,
	};

	enum KeyId {
		SPACE_key,
		ENTER_key,
	};

	enum ScreenPhaseId {
		TITLE_screen,
		STAGE_TO_screen,
		GAMEOVER_screen,
	};

public:
	//オブジェクト
	//リスト
	//list<unique_ptr<Base3D>> base3D;//3Dオブジェクト
	//list<unique_ptr<Base>> base;//2Dオブジェクト
	//ベクター
	vector<unique_ptr<BaseVector>> base;

	//コンストラクタ
	CTitle(CManager*);

	//デストラクタ
	~CTitle();

	int Update();//更新処理
	void Draw();//描画処理

	//マップManager
	std::unique_ptr<CMap> map;

	//CCamera* camera;//カメラオブジェクト

private:

	//タイトルロゴ画像ハンドル
	int TitleRogo_img{ -1 };

	//タイトルコマンド
	TitleCommandId TitleComId{ TitleCommandId::START_COM };

	//スクリーン状態
	ScreenPhaseId ScreenPhase{ ScreenPhaseId::TITLE_screen };

	//三角形の座標
	Point TrianglePos{ START_TRIANGLE_POS.x,START_TRIANGLE_POS.y };

	//キー入力配列
	bool PushKey[2]{ false, false };

	//カウント
	int TimerCnt{ 0 };

	//BGM サウンド用
	int BGM_Normal{ -1 };
	int BGM_GameOver{ -1 };

	//SE ステージスタート用
	int SE_StageStart{ -1 };

	//ステージスタートSEフラグ
	bool IsStageStartSE{ false };

	//ハイスコア保存用
	int HighScore{ 0 };
};