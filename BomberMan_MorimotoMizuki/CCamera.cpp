#pragma once
#include "CCamera.h"
#include "3D_function.h"

CCamera::CCamera() {
	GetMousePoint(&tmp_mouseX, &tmp_mouseY);//現在のマウスの位置情報を保存

	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;

	target.x = 0.0f;
	target.y = 0.0f;
	target.z = 0.0f;

	//カメラ距離
	SetCameraNearFar(0.051f, 1000.0f);
}

//矢印キーでカメラ移動(ターゲットは原点)
void CCamera::Action() {
	//左右移動
	if (CheckHitKey(KEY_INPUT_LEFT)) pos.x -= 1.0f;
	if (CheckHitKey(KEY_INPUT_RIGHT)) pos.x += 1.0f;

	//左シフト押しながら上下でz軸
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		if (CheckHitKey(KEY_INPUT_UP)) pos.z += 1.0f;
		if (CheckHitKey(KEY_INPUT_DOWN)) pos.z -= 1.0f;
	}
	else {
		if (CheckHitKey(KEY_INPUT_UP)) pos.y += 1.0f;
		if (CheckHitKey(KEY_INPUT_DOWN)) pos.y -= 1.0f;
	}

	//カメラの情報更新
	SetCameraPositionAndTarget_UpVecY(
		VGet(pos.x, pos.y, pos.z),
		VGet(target.x, target.y, target.z)
	);
}

//指定したオブジェクトを追跡
void CCamera::Action(Base3D* obj, list<unique_ptr<Base3D>>& base) {

	GetMousePoint(&mouseX, &mouseY);//マウスの位置情報取得

	//マウスの移動方向取得
	horizontal_angle -= (mouseX - tmp_mouseX) * 0.2f;
	vertical_angle += (mouseY - tmp_mouseY) * 0.2f;

	//縦回転 90°で回転を止める（カメラが反転する為）
	if (horizontal_angle >= 360)horizontal_angle = 0;
	if (horizontal_angle <= -360)horizontal_angle = 0;

	if (vertical_angle >= 90)vertical_angle = 89;
	if (vertical_angle <= -10)vertical_angle = -10;

	//注視点の計算
	Point3 target = obj->pos;
	target.y += LOOK_AT_HEIGHT;

	//計算用変数
	float sin_param, cos_param;//回転角
	Point3 p1, p2;//計算用位置

	//yz平面の回転
	sin_param = (float)sin(RADIAN(vertical_angle));
	cos_param = (float)cos(RADIAN(vertical_angle));
	p1.x = 0.0f;
	p1.y = sin_param * LOOK_AT_DISTANCE;
	p1.z = -cos_param * LOOK_AT_DISTANCE;

	//xz平面の回転
	sin_param = (float)sin(RADIAN(horizontal_angle));
	cos_param = (float)cos(RADIAN(horizontal_angle));
	p2.x = cos_param * p1.x - sin_param * p1.z;
	p2.y = p1.y;
	p2.z = sin_param * p1.x + cos_param * p1.z;

	//カメラの位置を計算
	pos.x = p2.x + target.x;
	pos.y = p2.y + target.y;
	pos.z = p2.z + target.z;

	//カメラの情報更新
	SetCameraPositionAndTarget_UpVecY(
		VGet(pos.x, pos.y, pos.z),
		VGet(target.x, target.y, target.z)
	);

	//マウスの位置を画面中央に移動
	SetMousePoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	//マウスの位置情報保存
	GetMousePoint(&tmp_mouseX, &tmp_mouseY);

	//カメラ情報の位置更新(グローバル)
	//CCameraInfo::SetPos(pos);
}

void CCamera::Draw() {
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%.0lf,%.0lf,%.0lf", pos.x, pos.y, pos.z);
}

void CCamera::Action(list<unique_ptr<Base3D>>& base) {}
CCamera::~CCamera() {}