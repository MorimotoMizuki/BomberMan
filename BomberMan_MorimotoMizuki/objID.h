#pragma once

//オブジェクトID
enum Obj_Id
{
	NONE = -1,
	BLOCK,
	CRASH_BLOCK,
	ITEM = 2,
	GOAL,
	PLAYER,
	BOMB,
	EXPLOSION,
	ENEMY,
};

//敵のID
enum Enemy_Id
{
	BALLOM = 10,	//バロム
	ONEAL,			//オニール
	DAHL,			//ダル
	MINVO,			//ミンボー
	KONDORIA,		//コンドリア
	OVAPEE,			//オバピー
	PASS,			//パース
	PONtAN,			//ポンタン
};

//アイテムのID : 効果 : 爆破すると出現する敵
enum Item_Id
{
	None = -1,
	FirePower,			//火力アップ					: オニール
	Bomb,				//爆弾の置ける数アップ			: バロム
	RemoteController,	//爆弾がBボタンで自由に爆発可能	: コンドリア
	Boots,				//移動速度アップ				: ダル
	BombPassing,		//爆弾の上を歩けるようになる	: オバピー
	WallPassing,		//壁の上を歩けるようになる		: ミンボー
	FlameBarrier,		//爆風で死ななくなる			: バース
	PerfectMan,			//30秒間無敵になる				: ポンタン
};

//オブジェクト表示順番
enum Pri_Id
{
	pGOAL,
	pITEM,
	pBLOCK,
	pBOMB,
	pEXPLOSION,
	pPLAYER,
	pENEMY,
};

//ゲーム状態
enum GamePhaseId
{
	IDLE,
	PLAING,
	GAMEOVER,
	GAMECLEAR,
};

//爆発エフェクトID
enum ExplosionEffectId
{
	SIZE0,
	SIZE1,
	SIZE2,
	SIZE3,

	CENTER = 0,
	VERTICAL = 4,
	HORIZONTAL = 8,
};

// 行動ID
enum Move_Id
{
	UP,		//上
	RIGHT,	//右
	DOWN,	//下
	LEFT,	//左
	SPACE,	//スペースキー
	LEFT_CLICK,	//マウス左クリック
	ENTER,	//エンターキー
	B_KEY,	//Bボタン
	ALL_KEY,//全てのキー入力
	NONE_KEY,
};

//プレイヤーステータス読み込み用
enum LoadStatus_Id
{
	LIFE,
	SPEED,
	BOMB_PUT_NUM,
	BOMB_LEVEL,
	SCORE,
	REMOTECONTROLLER,
	BOMB_PASS,
	WALL_PASS,
	FLAMEBARRIER,
	PERFECTMAN,
	STAGE_NUM,
};