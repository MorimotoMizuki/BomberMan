#pragma once

//オブジェクトID
enum Obj_Id
{
	PLAYER = 1,
	ENEMY,
	BOMB,
	BLOCK,
	EXPLOSION,
	ITEM,
};

//オブジェクト表示順番
enum Pri_Id
{
	pBLOCK,
	pENEMY,
	pBOMB,
	pEXPLOSION,
	pPLAYER,
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
	ALL_KEY,//全てのキー入力
};
