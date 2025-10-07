#pragma once
#include "3D_function.h"

//座標 + ベクトル
Point3 Add_Point_Vector(Point3 p0, Vector3 v0) { Point3 r;	r.x = p0.x + v0.x;	r.y = p0.y + v0.y;	r.z = p0.z + v0.z;	return r; }
//座標 - 座標
Vector3 Sub_Point_Point(Point3 p0, Point3 p1) { Vector3 r;	r.x = p0.x - p1.x;	r.y = p0.y - p1.y;	r.z = p0.z - p1.z;	return r; }
//ベクトル - ベクトル
Vector3 Sub_Vector_Vector(Vector3 p0, Vector3 p1) { Vector3 r;	r.x = p0.x - p1.x;	r.y = p0.y - p1.y;	r.z = p0.z - p1.z;	return r; }
//ベクトル + ベクトル
Vector3 Add_Vector_Vector(Vector3 v0, Vector3 v1) { Vector3 r;	r.x = v0.x + v1.x;	r.y = v0.y + v1.y;	r.z = v0.z + v1.z;	return r; }
//ベクトル × スカラー
Vector3 Mul_Vector_Scaler(Vector3 v, float s) { Vector3 r;	r.x = v.x * s;	r.y = v.y * s;	r.z = v.z * s;	return r; }
//ベクトルの長さ
float Vector_Length(Vector3 v) { float l = sqrtf((v.x * v.x + v.y * v.y + v.z * v.z));	return l; }
//ベクトルの正規化
Vector3 Vector_Normalize(Vector3 v) { float l = Vector_Length(v);	Vector3 n;	n.x = v.x / l;	n.y = v.y / l;	n.z = v.z / l;	return n; }
//ベクトルの長さを設定
Vector3 Vector_SetLength(Vector3 v, float length) { v = Vector_Normalize(v);	v = Mul_Vector_Scaler(v, length);	return v; }
//外積(3D)
Vector3 Vector3_Cross(Vector3 v1, Vector3 v2) { Vector3 v;	v.x = v1.y * v2.z - v1.z * v2.y;	v.y = v1.z * v2.x - v1.x * v2.z;	v.z = v1.x * v2.y - v1.y * v2.x;	return v; }
//内積(3D)
float Vector3_Dot(Vector3 v1, Vector3 v2) { float r;	r = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);	return r; }

//ベクトルに対して指定した角度に回転(x軸回転)
Vector3 Vector_Rotation_X(Vector3 vec, float angle) { Vector3 v;	vec = Vector_Normalize(vec);	v.x = 0.0f;	v.y = cos(RADIAN(angle)) * vec.y + sin(RADIAN(angle)) * vec.z;	v.z = -sin(RADIAN(angle)) * vec.y + cos(RADIAN(angle)) * vec.z;	return v; }
//指定した角度に回転(x軸回転)
Vector3 Vector_Rotation_X(float angle) { Vector3 v;	v.x = 0.0f;	v.y = cos(RADIAN(angle)) + sin(RADIAN(angle));	v.z = -sin(RADIAN(angle)) + cos(RADIAN(angle));	return v; }

//ベクトルに対して指定した角度に回転(y軸回転)
Vector3 Vector_Rotation_Y(Vector3 vec, float angle) { Vector3 v;		vec = Vector_Normalize(vec); v.x = cos(RADIAN(angle)) * vec.x - sin(RADIAN(angle)) * vec.z;	v.y = 0.0f;	v.z = sin(RADIAN(angle)) * vec.x + cos(RADIAN(angle)) * vec.z;	return v; }
//指定した角度に回転(y軸回転)
Vector3 Vector_Rotation_Y(float angle) { Vector3 v;	v.x = cos(RADIAN(angle)) - sin(RADIAN(angle));	v.y = 0.0f;	v.z = sin(RADIAN(angle)) + cos(RADIAN(angle));	return v; }

//ベクトルに対して指定した角度に回転(z軸回転)
Vector3 Vector_Rotation_Z(Vector3 vec, float angle) { Vector3 v;	vec = Vector_Normalize(vec);	v.x = cos(RADIAN(angle)) * vec.x + sin(RADIAN(angle)) * vec.y;	v.y = -sin(RADIAN(angle)) * vec.x + cos(RADIAN(angle)) * vec.y;	v.z = 0.0f;	return v; }
//指定した角度に回転(z軸回転)
Vector3 Vector_Rotation_Z(float angle) { Vector3 v;	v.x = cos(RADIAN(angle)) + sin(RADIAN(angle));	v.y = -sin(RADIAN(angle)) + cos(RADIAN(angle));	v.z = 0.0f;	return v; }

//AABB判定
bool HitCheck_AABB(Base3D* a, Base3D* b) {
	MINMAX A(a->pos, a->b_box);
	MINMAX B(b->pos, b->b_box);

	if (
		A.min.x <= B.max.x &&
		A.max.x >= B.min.x &&
		A.min.y <= B.max.y &&
		A.max.y >= B.min.y &&
		A.min.z <= B.max.z &&
		A.max.z >= B.min.z
		)
		return true;
	return false;
}
bool HitCheck_AABB(MINMAX a, MINMAX b) {
	if (
		a.min.x < b.max.x &&
		a.max.x > b.min.x &&
		a.min.y < b.max.y &&
		a.max.y > b.min.y &&
		a.min.z < b.max.z &&
		a.max.z > b.min.z
		)
		return true;
	return false;
}