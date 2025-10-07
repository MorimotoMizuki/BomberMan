#pragma once
#include "objBase.h"
#include "2D_function.h"
#include "3D_function.h"

//オブジェクトソート関数(vector)
void ObjSort_Quick(vector<unique_ptr<BaseVector>>&, int, int);

//指定範囲の乱数を取得(min,max)
int Range_Random_Number(int, int);
float Range_Random_NumberF(float, float);

//全てのキーが、離されるまで待機
void WAIT_RELEASE_ALL_KEY();

//全てのキーが、離されるまで待機（タイプ指定 DX_CHECKINPUT_KEY : DX_CHECKINPUT_PAD : DX_CHECKINPUT_MOUS)
void WAIT_RELEASE_ALL_KEY(int);

//数値の描画（画像ハンドル, 一桁目の位置x, 一桁目の位置y, 値, 文字サイズw, 文字サイズh)
void DrawNum(int, int, int, int, int, int);

//リストから指定オブジェクトの取得
Base* Get_obj(list<unique_ptr<Base>>&, int);
Base3D* Get_obj(list<unique_ptr<Base3D>>&, int);
BaseVector* Get_obj(vector<unique_ptr<BaseVector>>&, int);

//指定したオブジェクトの座標取得
Point Get_Point(list<unique_ptr<Base>>&, int);
Point Get_Point(vector<unique_ptr<BaseVector>>&, int);

//指定したオブジェクト以外をリストから削除
void Del_SpecifiedObj(list<unique_ptr<Base>>&, int);
void Del_SpecifiedObj(vector<unique_ptr<Base>>&, int);

//指定オブジェクトをリストから削除(FLAGをfalseにする)
void Del_ListObj(list<unique_ptr<Base>>&, int);
void Del_ListObj(vector<unique_ptr<Base>>&, int);

//指定オブジェクトをリストから削除範囲(FLAGをfalseにする)
void Del_ListObj(list<unique_ptr<Base>>&, int, int);
void Del_ListObj(vector<unique_ptr<Base>>&, int, int);