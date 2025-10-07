#pragma once
#include "objBase.h"

//座標 + ベクトル
Point Add_Point_Vector(Point, Vector);
//座標 - 座標
Vector Sub_Point_Point(Point, Point);
//座標 - 座標(x,y,x,y)
Vector Sub_Point_Point(float, float, float, float);
//ベクトル - ベクトル
Vector Sub_Vector_Vector(Vector, Vector);
//ベクトル + ベクトル
Vector Add_Vector_Vector(Vector, Vector);
//ベクトル × スカラー
Vector Mul_Vector_Scaler(Vector, float);
//ベクトルの長さ
float Vector_Length(Vector);
//ベクトルの正規化
Vector Vector_Normalize(Vector);
//ベクトルの長さを設定
Vector Vector_SetLength(Vector, float);
//ベクトルの回転
Vector Vector_Rotation(Vector, float);
//内積
float Dot(Vector, Vector);


//球判定(位置A,半径A,位置B,半径B)
bool HitCheck_Sphere(Point3, float, Point3, float);
//矩形判定(x1,y1,x2,y2,w,h)
bool HitCheck_box(float, float, float, float, int, int);
//矩形判定２(x1,y1,x2,y2,w1,h1,w2,h2)
bool HitCheck_box(float, float, float, float, int, int, int, int);
//矩形判定3(オブジェクト,オブジェクト)
//画像サイズ(ImgWidth,ImgHeight)で判定
bool HitCheck_box(Base*, Base*);
bool HitCheck_box(BaseVector*, BaseVector*);

//ID指定矩形判定
//画像サイズ(ImgWidth,ImgHeight)で判定
bool HitCheck_box(vector<unique_ptr<BaseVector>>&, BaseVector*, int);
//当たったオブジェクトを受け取る
BaseVector* HitCheck_box_GetObj(vector<unique_ptr<BaseVector>>&, BaseVector*, int);


//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)
void HitCheck_Box_Circle(Base*, Base*, float);
//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)
bool HitCheck_Box_Circle2(Base*, Base*, float,int);

//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)
//円オブジェクトのベクトルを更新
void HitCheck_Box_Circle(BaseVector*, BaseVector*, float);
//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)
bool HitCheck_Box_Circle2(BaseVector*, BaseVector*, float);

//円とカプセルの判定


//二点間距離
float DistanceF(float, float, float, float);
float DistanceF(Point3, Point3);