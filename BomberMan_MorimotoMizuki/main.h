//DX Library Copyright (C) 2001-2025 Takumi Yamada.
//Dxlib_lib_ver6.02 2025 Kcomテンプレート Shinya Wakisaka.
#pragma once
#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <list>
#include <vector>
#include <memory>
#include <time.h>

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


//2D用
 struct Point {
	float x, y;
};
 struct Vector {
	float x, y;
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
	 
	 //移動速度
	 float speed;
	 //設置可能な爆弾の数
	 int bombPutNum;
	 //爆弾の威力のレベル
	 int bombLevel;

 };

//プレイヤーのステータス
extern PlayerStatus gPlayerStatus;
//現在設置している爆弾の数
extern int gNowBombNum;

//カメラ情報クラス(main.h)
//class CCameraInfo {
//private:
//	static Point3 pos;//カメラの位置
//public:
//	static Point3 GetPos() { return pos; }//カメラの位置情報取得
//	static void SetPos(Point3 p) { pos = p; }//カメラの位置情報保存
//};