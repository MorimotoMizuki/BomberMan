#pragma once
#include "DxLib.h"
#include "main.h"
#include <list>
#include <memory>

using namespace std;

//キャラクターベースクラス
class Character {
public:
	int img{ -1 };//画像

	Point pos{ 0,0 };//位置
	Vector vec{ 0,0 };//移動ベクトル

	int CutX{ 0 };//画像切り取り位置x
	int CutY{ 0 };//画像切り取り位置y

	int ImgWidth{ 0 };//画像の横サイズ
	int ImgHeight{ 0 };//画像の縦サイズ

	int Dir{ 0 };//オブジェクトの向き

	int sound[8]{ -1,-1,-1,-1,-1,-1,-1,-1 };

	~Character() { 
		DeleteGraph(img);
		for (int i = 0; i < 8; i++) DeleteSoundMem(sound[i]);
	}
};

//キャラクターベース3D
class Character3D :public Character{
public:
	int model{ -1 };//モデル

	Point3 pos{ 0,0,0 };//位置
	Vector3 vec{ 0,0,0 };//移動ベクトル

	ObjSize size{ 0,0,0 };//オブジェクトサイズ

	Vector3 b_box{ 0,0,0 };//バウンディングボックスサイズ(縦,横,高さ)

	//アニメーション処理用
	int AttachIndex{ -1 };//アニメーションアタッチNo
	float animTime{ 0 };
	//アタッチ番号取得
	int Attach_num_Get(int num) { return MV1AttachAnim(model, num, -1, false); }

	//バウンディングボックスサイズセット
	void Set_Bounding_Box_Size(float x, float y, float z) {
		b_box.x = x;
		b_box.y = y;
		b_box.z = z;
	}

	~Character3D() { MV1DeleteModel(model); };
};
