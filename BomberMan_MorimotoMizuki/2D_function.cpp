#pragma once
#include "2D_function.h"

//座標 + ベクトル
Point Add_Point_Vector(Point p0, Vector v0) {
	Point r;	
	r.x = p0.x + v0.x;	
	r.y = p0.y + v0.y;	
	return r; 
}
//座標 - 座標
Vector Sub_Point_Point(Point p0, Point p1) { Vector r;	r.x = p0.x - p1.x;	r.y = p0.y - p1.y;	return r; }
//座標 - 座標(x,y,x,y)
Vector Sub_Point_Point(float x1, float y1, float x2, float y2) {	Vector r;	r.x = x1 - x2;	r.y = y1 - y2;	return r;}
//ベクトル - ベクトル
Vector Sub_Vector_Vector(Vector p0, Vector p1) { Vector r;	r.x = p0.x - p1.x;	r.y = p0.y - p1.y;	return r; }
//ベクトル + ベクトル
Vector Add_Vector_Vector(Vector v0, Vector v1) { 
	Vector r;	
	r.x = v0.x + v1.x;	
	r.y = v0.y + v1.y;	
	return r; 
}
//ベクトル × スカラー
Vector Mul_Vector_Scaler(Vector v, float s) { Vector r;	r.x = v.x * s;	r.y = v.y * s;	return r; }
//ベクトルの長さ
float Vector_Length(Vector v) { 
	float l = sqrtf((v.x * v.x + v.y * v.y));
	return l; 
}
//ベクトルの正規化
Vector Vector_Normalize(Vector v) { float l = Vector_Length(v);	Vector n;	n.x = v.x / l;	n.y = v.y / l;	return n; }
//ベクトルの長さを設定
Vector Vector_SetLength(Vector v, float length) { v = Vector_Normalize(v);	v = Mul_Vector_Scaler(v, length);	return v; }
//ベクトルの回転
Vector Vector_Rotation(Vector v, float angle) {	Vector r;	float l = Vector_Length(v);	v = Vector_Normalize(v);	r.x = v.x * cos(RADIAN(angle)) - v.y * sin(RADIAN(angle));	r.y = v.x * sin(RADIAN(angle)) + v.y * cos(RADIAN(angle));	return r;}
//内積
float Dot(Vector v1, Vector v2) {
	float r;
	v1 = Vector_Normalize(v1);
	v2 = Vector_Normalize(v2);

	r = v1.x * v2.x + v1.y * v2.y;

	return r;
}

//球判定(位置A,半径A,位置B,半径B)
bool HitCheck_Sphere(Point3 A, float RA, Point3 B, float RB) {
	if (DistanceF(A, B) < RA + RB)
		return true;
	return false;
}
//矩形判定(x1,y1,x2,y2,w,h)
bool HitCheck_box(float x1, float y1, float x2, float y2, int w, int h) {
	if (x1 < x2 + w && x2 < x1 + w && y1 < y2 + h && y2 < y1 + h)
		return 1;
	return 0;
}
//矩形判定２(x1,y1,x2,y2,w1,h1,w2,h2)
bool HitCheck_box(float x1, float y1, float x2, float y2, int w1, int h1, int w2, int h2) {
	if (x1 < x2 + w2 && x2 < x1 + w1 &&	y1 < y2 + h2 && y2 < y1 + h1)
		return 1;
	return 0;
}
//矩形判定3(オブジェクト,オブジェクト)
//画像サイズ(ImgWidth,ImgHeight)で判定
bool HitCheck_box(Base* a, Base* b){
	if (a->pos.x < b->pos.x + b->ImgWidth &&
		b->pos.x < a->pos.x + a->ImgWidth &&
		a->pos.y < b->pos.y + b->ImgHeight &&
		b->pos.y < a->pos.y + a->ImgHeight)
		return 1;
	return 0;
}
bool HitCheck_box(BaseVector* a, BaseVector* b) {
	if (a->pos.x < b->pos.x + b->ImgWidth &&
		b->pos.x < a->pos.x + a->ImgWidth &&
		a->pos.y < b->pos.y + b->ImgHeight &&
		b->pos.y < a->pos.y + a->ImgHeight)
		return true;
	return false;
}

//ID指定矩形判定
//画像サイズ(ImgWidth,ImgHeight)で判定
bool HitCheck_box(vector<unique_ptr<BaseVector>>& base, BaseVector* a, int ID) {
	for (int i = 0; i < base.size(); i++) {
		if (base[i]->ID == ID) {
			if (HitCheck_box(a, base[i].get())) {
				return true;
			}
		}
	}
	return false;
}

//当たったオブジェクトを受け取る
//画像サイズ(ImgWidth,ImgHeight)で判定
BaseVector* HitCheck_box_GetObj(vector<unique_ptr<BaseVector>>& base, BaseVector* a, int ID) {
	for (int i = 0; i < base.size(); i++) {
		if (base[i]->ID == ID) {
			if (HitCheck_box(a, base[i].get())) {
				return base[i].get();
			}
		}
	}
	return a;
}

//円と矩形判定用関数（矩形の一番近い位置を計算）
float Circle_Center_With_Closest_Point(float a, float b, float c)
{
	if (a < b) {
		return b;
	}
	else if (a > c) {
		return c;
	}
	else {
		return a;
	}
}
//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)
void HitCheck_Box_Circle(Base* a, Base* b, float radius) {
	Point pc{ a->pos.x + a->vec.x + a->ImgWidth / 2,a->pos.y + a->vec.y + a->ImgHeight / 2 };//中心座標計算
	Point n{ -1,-1 };//矩形の一番近い座標
	n.x = Circle_Center_With_Closest_Point(pc.x, b->pos.x, b->pos.x + b->ImgWidth);
	n.y = Circle_Center_With_Closest_Point(pc.y, b->pos.y, b->pos.y + b->ImgHeight);

	float l = DistanceF(pc.x, pc.y, n.x, n.y);//一番近い座標から中心までの距離
	if (l < radius)//半径radiusで判定
	{
		Vector v{ n.x - pc.x, n.y - pc.y };//方向ベクトル
		v = Vector_Normalize(v);//方向ベクトル正規化
		v = Vector_SetLength(v, radius - l);
		a->vec.x -= (int)v.x;
		a->vec.y -= (int)v.y;
	}
}
bool HitCheck_Box_Circle2(Base* a, Base* b, float radius,int ) {
	Point pc{ a->pos.x + a->vec.x + a->ImgWidth / 2,a->pos.y + a->vec.y + a->ImgHeight / 2 };//中心座標計算
	Point n{ -1,-1 };//矩形の一番近い座標
	n.x = Circle_Center_With_Closest_Point(pc.x, b->pos.x, b->pos.x + b->ImgWidth);
	n.y = Circle_Center_With_Closest_Point(pc.y, b->pos.y, b->pos.y + b->ImgHeight);

	float l = DistanceF(pc.x, pc.y, n.x, n.y);//一番近い座標から中心までの距離
	if (l < radius)//半径radiusで判定
	{
		return true;
	}
	return false;
}
void HitCheck_Box_Circle(BaseVector* a, BaseVector* b, float radius) {
	Point pc{ a->pos.x + a->vec.x + a->ImgWidth / 2,a->pos.y + a->vec.y + a->ImgHeight / 2 };//中心座標計算
	Point n{ -1,-1 };//矩形の一番近い座標
	n.x = Circle_Center_With_Closest_Point(pc.x, b->pos.x, b->pos.x + b->ImgWidth);
	n.y = Circle_Center_With_Closest_Point(pc.y, b->pos.y, b->pos.y + b->ImgHeight);

	float l = DistanceF(pc.x, pc.y, n.x, n.y);//一番近い座標から中心までの距離
	if (l < radius)//半径radiusで判定
	{
		Vector v{ n.x - pc.x, n.y - pc.y };//方向ベクトル
		v = Vector_Normalize(v);//方向ベクトル正規化
		v = Vector_SetLength(v, radius - l);
		a->vec.x -= (int)v.x;
		a->vec.y -= (int)v.y;
	}
}
bool HitCheck_Box_Circle2(BaseVector* a, BaseVector* b, float radius) {
	Point pc{ a->pos.x + a->vec.x + a->ImgWidth / 2,a->pos.y + a->vec.y + a->ImgHeight / 2 };//中心座標計算
	Point n{ -1,-1 };//矩形の一番近い座標
	n.x = Circle_Center_With_Closest_Point(pc.x, b->pos.x, b->pos.x + b->ImgWidth);
	n.y = Circle_Center_With_Closest_Point(pc.y, b->pos.y, b->pos.y + b->ImgHeight);

	float l = DistanceF(pc.x, pc.y, n.x, n.y);//一番近い座標から中心までの距離
	if (l < radius)//半径radiusで判定
	{
		return true;
	}
	return false;
}

//二点間距離
float DistanceF(float a, float b, float c, float d)
{
	return sqrtf((a - c) * (a - c) + (b - d) * (b - d));
}
float DistanceF(Point3 A, Point3 B) {
	float r;
	r = sqrtf((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y) + (A.z - B.z) * (A.z - B.z));
	return r;
}