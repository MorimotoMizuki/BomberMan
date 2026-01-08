#pragma once
#include "objBase.h"
#include"objID.h"
#include "2D_function.h"
#include "3D_function.h"

#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<map>

//文字列分割関数 : 指定した文字で文字列を分割する
//(文字列, 分割文字)
vector<string> split(string& input, char delimiter);

//キー入力チェック関数
bool Key_Check(Move_Id);
//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)　脇坂先生のやつに distance のずれを追加
void HitCheck_Box_Circle(BaseVector* a, BaseVector* b, float radius, float distance);

//プレイヤーのステータスをリセット
void ResetPlayerStatus();

//サウンド再生関数
void My_PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag, int MusicVolume = 255);