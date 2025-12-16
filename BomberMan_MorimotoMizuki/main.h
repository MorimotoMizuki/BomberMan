//DX Library Copyright (C) 2001-2025 Takumi Yamada.
//Dxlib_lib_ver6.02 2025 Kcomテンプレート Shinya Wakisaka.
#pragma once
#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <list>
#include <vector>
#include <memory>
#include <time.h>
#include <tuple>

#include "objID.h"

using namespace std;

//画像サイズ
#define IMGSIZE64 64
#define IMGSIZE32 32
#define IMGSIZE16 16

//ラジアン変換マクロ
#define RADIAN(_deg) ((float)M_PI*(_deg)/180.0f)
#define DEGREE(_rad) ((float)(_rad)*180.0f/M_PI)

constexpr auto WINDOW_HEADER = 96;	//画面上のサイズ

//基本ウィンドウサイズ
constexpr auto WINDOW_WIDTH = 1024;	//横幅 64*16
constexpr auto WINDOW_HEIGHT = IMGSIZE64 * 13 + WINDOW_HEADER;	//縦幅 64*13+48

//マップ全体のマップチップ数
constexpr auto MAP_CHIP_W = 31;
constexpr auto MAP_CHIP_H = 13;

//描画するマップチップ数(描画画面のサイズ)
constexpr auto DRAW_CHIP_W = 15;
constexpr auto DRAW_CHIP_H = 13;

//マップチップのサイズ
constexpr auto CHIP_SIZE = IMGSIZE64;

//ステージの合計数
constexpr auto STAGE_SUM = 3;

//爆弾の火力の最大値
constexpr auto MAX_FIRE_POWER = 5;

//爆弾を設置できる最大値
constexpr auto MAX_PUT_BOMB_NUM = 10;

//敵の種類
constexpr auto ENEMY_VARIATION = 8;

//2D用
 struct Point {
	float x, y;
};
 struct Vector {
	float x, y;
};

 struct MapPoint {
	 int x, y;
 };

//3D用
struct Point3 {
	float x, y, z;
};
struct Vector3 {
	float x, y, z;
};
 struct ObjSize {
	float h, w, d;
};

 struct PlayerStatus {
	 
	 //ライフ
	 int life;
	 //移動速度
	 float speed;
	 //設置可能な爆弾の数
	 int bombPutNum;
	 //爆弾の威力のレベル
	 int bombLevel;
	 //スコア
	 int score;
 };

//プレイヤーのステータス
extern PlayerStatus gPlayerStatus;
//現在設置している爆弾の数
extern int gNowBombNum;

//現在のマップ[y][x]
extern int gNowMap[MAP_CHIP_H + 1][MAP_CHIP_W + 1];

//ゲームの状態
extern GamePhaseId gGamePhase;

//現在のステージ番号
extern int gNowStageNum;

//敵を倒した数
extern int gKillEnemyNum;

//敵の描画順番
extern int gEnemyPri;

//カメラ情報クラス(main.h)
//class CCameraInfo {
//private:
//	static Point3 pos;//カメラの位置
//public:
//	static Point3 GetPos() { return pos; }//カメラの位置情報取得
//	static void SetPos(Point3 p) { pos = p; }//カメラの位置情報保存
//};