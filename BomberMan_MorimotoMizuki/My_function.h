#pragma once
#include "objBase.h"
#include"objID.h"
#include "2D_function.h"
#include "3D_function.h"

//キー入力チェック関数
bool Key_Check(Move_Id);
//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)　脇坂先生のやつに distance のずれを追加
void HitCheck_Box_Circle(BaseVector* a, BaseVector* b, float radius, float distance);

//プレイヤーのステータスをリセット
void ResetPlayerStatus();