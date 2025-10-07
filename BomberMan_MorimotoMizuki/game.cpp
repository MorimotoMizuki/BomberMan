//ゲームシーン
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"

#include"obj.h"

//プレイヤーのステータス
PlayerStatus gPlayerStatus = {
	6.0f,	//speed
	1,		//bombPutNum
	1,		//bombLevel
};
//現在の設置している爆弾の数
int gNowBombNum = 0;

//コンストラクタ
CGame::CGame(CManager* p) :CScene(p)
{
	base.emplace_back((unique_ptr<BaseVector>)new CPlayer());

	Point bp{ 0,WINDOW_HEADER };
	for (int i = 0; i < 16; i++)
	{
		base.emplace_back((unique_ptr<BaseVector>)new CBlock(bp,Obj_Id::NORMALBLOCK));
		bp.x += 64;
	}

}

//更新処理
int CGame::Update()
{
	//ヘッダーの背景
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEADER, GetColor(173, 173, 173), 1);
	//ゲーム背景
	DrawBox(0, WINDOW_HEADER, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(56, 135, 0), 1);

	//更新処理
	for (int i = 0; i < base.size(); i++)
		base[i]->Action(base);

	//削除処理
	for (auto i = base.begin(); i != base.end();)
		(*i)->FLAG ? i++ : i = base.erase(i);

	//オブジェクトのソート処理(クイックソート)
	ObjSort_Quick(base, 0, base.size() - 1);

	//listオブジェクトの更新処理	
	//for (auto i = base.begin(); i != base.end(); i++)
	//	(*i)->Action(base);

	return 0;
}

//描画処理
void CGame::Draw()
{
	//オブジェクト個数
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Object_Count = %d", base.size());
	
	for (int i = 0; i < base.size(); i++)
		if(base[i]->FLAG) base[i]->Draw();

	//listオブジェクトの描画
	//for (auto i = base.begin(); i != base.end(); i++)
	//	if ((*i)->FLAG) (*i)->Draw();

	//3D軸の描画
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 0, 100),
	//	0x0000ff
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(100, 0, 0),
	//	0xff0000
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 100, 0),
	//	0x00ff00
	//);
}

CGame::~CGame()
{

}
