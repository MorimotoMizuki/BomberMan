#pragma once
#include "library_function.h"
#include <random>

//オブジェクトソート関数(vector)
int Pivot(vector<unique_ptr<BaseVector>>& base, int low, int hight) {
	int pivot = base[hight]->pri;//最後のpri値をピボットにする
	int i = low - 1;//小さい要素インデックス

	for (int j = low; j < hight; j++) {
		if (base[j]->pri < pivot) {
			i++;
			swap(base[i], base[j]);//要素の入れ替え
		}
	}
	swap(base[i + 1], base[hight]);
	return i + 1;
}
void ObjSort_Quick(vector<unique_ptr<BaseVector>>& base, int low, int hight) {
	if (low < hight) {
		int pivot_position = Pivot(base, low, hight);
		ObjSort_Quick(base, low, pivot_position - 1);//左側
		ObjSort_Quick(base, pivot_position + 1, hight);//右側
	}
}


//指定範囲の乱数を取得(min,max)
int Range_Random_Number(int min, int max)
{
	random_device rd;
	default_random_engine engine(rd());
	uniform_int_distribution<int> dis(min, max);

	return dis(engine);
}

//指定範囲の乱数を取得(min,max)(32bit版)
float Range_Random_NumberF(float min, float max)
{
	random_device rd;
	mt19937 engine(rd());
	uniform_real_distribution<float> dis(min, max);

	return dis(engine);
}

//全てのキーが、離されるまで待機
void WAIT_RELEASE_ALL_KEY()
{
	while (true)
	{
		if (CheckHitKeyAll() == 0) break;
	}
}

//全てのキーが、離されるまで待機（タイプ指定）
//DX_CHECKINPUT_KEY : DX_CHECKINPUT_PAD : DX_CHECKINPUT_MOUSE
void WAIT_RELEASE_ALL_KEY(int type)
{
	while (true)
	{
		if (CheckHitKeyAll(type) == 0) break;
	}
}

//数値の描画（画像ハンドル, 一桁目の位置x, 一桁目の位置y, 値, 文字サイズw, 文字サイズh)
void DrawNum(int img, int x, int y, int num, int w, int h)
{
	//桁計算（表示位置に使用）
	int digits{ 0 };
	int tmp_num = num;

	while (true)
	{
		DrawRectGraph(
			x - w / 2 - digits * w, y,
			(int)tmp_num % 10 * w, 0,
			w, h,
			img,
			true
		);
		tmp_num = tmp_num / 10;
		digits++;
		if (tmp_num == 0) break;
	}
}

//リストから指定オブジェクトの取得
Base* Get_obj(list<unique_ptr<Base>>& base, int ID)
{
	for (auto i = base.begin(); i != base.end(); i++)
	{
		if ((*i)->ID == ID)
		{
			return (*i).get();
		}
	}
	return nullptr;
}
//リストから指定オブジェクトの取得
Base3D* Get_obj(list<unique_ptr<Base3D>>& base, int ID)
{
	for (auto i = base.begin(); i != base.end(); i++)
	{
		if ((*i)->ID == ID)
		{
			return (*i).get();
		}
	}
	return nullptr;
}
BaseVector* Get_obj(vector<unique_ptr<BaseVector>>& base, int ID)
{
	for (int i = 0; i < base.size(); i++) {
		if (base[i]->ID == ID) {
			return base[i].get();
		}
	}

	return base[0].get();
}

//指定したオブジェクトの座標取得
Point Get_Point(list<unique_ptr<Base>>& base, int ID)
{
	Point p{ 0,0 };

	for (auto i = base.begin(); i != base.end(); i++) {
		if ((*i)->ID == ID) return (*i)->pos;
	}
	return p;
}
Point Get_Point(vector<unique_ptr<Base>>& base, int ID)
{
	Point p{ 0,0 };

	for (int i = 0; i<base.size();i++) {
		if (base[i]->ID == ID) p = base[i]->pos;
	}
	return p;
}

//指定したオブジェクト以外をリストから削除
void Del_SpecifiedObj(list<unique_ptr<Base>>& base, int ID) {
	for (auto i = base.begin(); i != base.end(); i++) {
		if ((*i)->ID != ID) {
			(*i)->FLAG = false;
			(*i)->ID = -1;
		}
	}
}
void Del_SpecifiedObj(vector<unique_ptr<Base>>& base, int ID) {
	for (auto i = 0; i<base.size();i++){
		if (base[i]->ID != ID) {
			base[i]->FLAG = false;
			base[i]->ID = -1;
		}
	}
}

//指定オブジェクトをリストから削除(FLAGをfalseにする)
void Del_ListObj(list<unique_ptr<Base>>& base, int ID) {
	for (auto i = base.begin(); i != base.end(); i++) {
		if ((*i)->ID == ID) {
			(*i)->FLAG = false;
			(*i)->ID = -1;
		}
	}
}
void Del_ListObj(vector<unique_ptr<Base>>& base, int ID) {
	for (int i = 0; i < base.size(); i++) {
		if (base[i]->ID == ID) {
			base[i]->FLAG = false;
			base[i]->ID = -1;
		}
	}
}

//指定オブジェクトをリストから削除範囲(FLAGをfalseにする)
void Del_ListObj(list<unique_ptr<Base>>& base, int ID, int ID2) {
	for (auto i = base.begin(); i != base.end(); i++) {
		if ((*i)->ID >= ID && (*i)->ID <= ID2) {
			(*i)->FLAG = false;
			(*i)->ID = -1;
		}
	}
}
void Del_ListObj(vector<unique_ptr<Base>>& base, int ID, int ID2) {
	for (int i = 0; i < base.size();i++){
		if (base[i]->ID >= ID && base[i]->ID <= ID2) {
			base[i]->FLAG = false;
			base[i]->ID = -1;
		}
	}
}

/*
* 旧乱数生成関数
//int Range_Random_Number(int min, int max)
//{
//	return (rand() % (max - min + 1)) + min;
//}
*/
