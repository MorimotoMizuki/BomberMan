#pragma once
#include "objBase.h"
#include "2D_function.h"

//最大最小
//(Point3 A, Point3 B) A → B 間の最大最小
//最大値最小値
struct MINMAX{
	Vector3 min{ 0,0,0 };	Vector3 max{ 0,0,0 };
	//Point3 to Point3
	MINMAX(Point3 a, Point3 b) {min.x = a.x;min.y = a.y;min.z = a.z;max.x = a.x + b.x;max.y = a.y + b.y;max.z = a.z + b.z;}
	//Vector3 to Vector3
	MINMAX(Vector3 a, Vector3 b) {min.x = a.x;min.y = a.y;min.z = a.z;max.x = a.x + b.x;max.y = a.y + b.y;max.z = a.z + b.z;}
	//Point3 to Vector3
	MINMAX(Point3 a, Vector3 b) {min.x = a.x;min.y = a.y;min.z = a.z;max.x = a.x + b.x;max.y = a.y + b.y;max.z = a.z + b.z;}
};

//座標 + ベクトル
Point3 Add_Point_Vector(Point3, Vector3);
//座標 - 座標
Vector3 Sub_Point_Point(Point3, Point3);
//ベクトル - ベクトル
Vector3 Sub_Vector_Vector(Vector3, Vector3);
//ベクトル + ベクトル
Vector3 Add_Vector_Vector(Vector3, Vector3);
//ベクトル × スカラー
Vector3 Mul_Vector_Scaler(Vector3, float);
//ベクトルの長さ
float Vector_Length(Vector3);
//ベクトルの正規化
Vector3 Vector_Normalize(Vector3);
//ベクトルの長さを設定
Vector3 Vector_SetLength(Vector3, float);
//内積(3D)
float Vector3_Dot(Vector3, Vector3);
//外積(3D)
Vector3 Vector3_Cross(Vector3, Vector3);
//ベクトルに対して指定した角度に回転(x軸回転)
Vector3 Vector_Rotation_X(Vector3, float);
//指定した角度に回転(x軸回転)
Vector3 Vector_Rotation_X(float);
//ベクトルに対して指定した角度に回転(y軸回転)
Vector3 Vector_Rotation_Y(Vector3, float);
//指定した角度に回転(y軸回転)
Vector3 Vector_Rotation_Y(float);
//ベクトルに対して指定した角度に回転(z軸回転)
Vector3 Vector_Rotation_Z(Vector3, float);
//指定した角度に回転(z軸回転)
Vector3 Vector_Rotation_Z(float);

//AABB判定(オブジェクト,オブジェクト)
bool HitCheck_AABB(Base3D*, Base3D*);
//AABB判定
bool HitCheck_AABB(MINMAX, MINMAX);
