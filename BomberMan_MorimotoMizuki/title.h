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

	//パスワードの最大文字数
	static constexpr auto PASSWORD_MAX = 20;

	//エンディングからステージ移動するフレーム数
	static constexpr auto ENDING_TO_STAGE_FRAME = 3 * 60;

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
		CONTINUE_screen,
		ENDING_screen,
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

	void LoadStatus();	//ステータスデータ読み込み

	//パスワード入力
	void UpdatePasswordInput();

	//パスワード検証
	bool IsValidPassword(std::string& password);

	//毎フレーム更新 : キー
	void UpdateKeyState();

	//押した瞬間判定 : キー
	bool IsKeyTrigger(int key);

	//マップManager
	std::unique_ptr<CMap> map;

	//CCamera* camera;//カメラオブジェクト

private:

	//タイトルロゴ画像ハンドル
	int TitleRogo_img{ -1 };

	//レンガ画像ハンドル
	int Renga_img{ -1 };

	//エンディングプレイヤー画像ハンドル
	int EndingPlayer_img[8]{ -1,-1, -1, -1, -1, -1, -1, -1 };

	//エンディングのプレイヤーの座標
	Point EndingPlayerPos{ 0,0 };

	//エンディングのプレイヤーのアニメーション用
	int AnimIndex{ 0 };
	int AnimCnt{ 0 };
	int AnimMaxIndex{ 0 };
	bool IsRoadRunner{ false };	//ロードランナーフラグ
	int TotalMovePosX{ 0 };

	int EndingEndCnt{ 0 };

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
	int BGM_Ending{ -1 };

	//SE サウンド用
	int SE_StageStart{ -1 }; //ステージスタート用
	int SE_PlayerWalk{ -1 }; //プレイヤー歩行SE

	//ステージスタートSEフラグ
	bool IsStageStartSE{ false };

	//入力用パスワード
	std::string Password;

	//今フレーム
	char KeyState[256];
	//前フレーム
	char PrevKeyState[256];

	//パスワード検証後のフラグ
	bool IsPassword{ false };
};