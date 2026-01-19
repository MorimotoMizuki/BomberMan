#include"obj.h"
#include"function.h"

CSpecial_Item_Manager::CSpecial_Item_Manager(Special_Item_Id special_item_id)
{
	Special_Item_ID = special_item_id;

	SetItemCreateFunction(); //アイテムの生成条件関数の設定

	//ゴーデス用
	OuterRoute.clear(); //一応クリアしておく
	//上辺
	for (int x = left; x <= right; x++)		OuterRoute.push_back({ x, top });
	//右辺
	for (int y = top + 1; y <= bottom; y++) OuterRoute.push_back({ right, y });
	//下辺
	for (int x = right - 1; x >= left; x--) OuterRoute.push_back({ x, bottom });
	//左辺
	for (int y = bottom - 1; y > top; y--)	OuterRoute.push_back({ left, y });
}

int CSpecial_Item_Manager::Action(vector<unique_ptr<BaseVector>>& base)
{
	//出現済みフラグがtrueの場合は終了
	if (IsPopItem[static_cast<int>(Special_Item_ID)])
		return 0;

	if (ItemCreateFunctions.contains(Special_Item_ID)) {

		//アイテムの生成条件を満たしていた場合
		if (ItemCreateFunctions[Special_Item_ID](base))
		{
			//プレイヤーを取得
			CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);

			MapPoint s_p{ 0,0 };
			while (true)
			{
				s_p = { Range_Random_Number(1, MAP_CHIP_W), Range_Random_Number(1, MAP_CHIP_H) };

				//プレイヤーの座標と同じ座標の場合は再計算
				if (s_p.x == p->SystemPos.x && s_p.y == p->SystemPos.y)
					continue;

				//空白の場合は生成座標確定
				if (gNowMap[s_p.y][s_p.x] == Obj_Id::NONE) break;
			}

			Point c_p{ s_p.x * CHIP_SIZE, s_p.y * CHIP_SIZE + WINDOW_HEADER };

			//隠れアイテム生成
			base.emplace_back((unique_ptr<BaseVector>) new CSpecial_Item(c_p, s_p, Special_Item_ID));

			IsPopItem[static_cast<int>(Special_Item_ID)] = true; //アイテムの出現フラグtrue
		}
	}

	return 0;
}

void CSpecial_Item_Manager::Draw()
{

}

CSpecial_Item_Manager::~CSpecial_Item_Manager()
{

}

//各アイテムの生成関数設定
void CSpecial_Item_Manager::SetItemCreateFunction()
{
	ItemCreateFunctions[Special_Item_Id::B_Panel]		= [&](vector<unique_ptr<BaseVector>>& base) -> bool { return B_PanelCreateAction(base); };
	ItemCreateFunctions[Special_Item_Id::Goddess]		= [&](vector<unique_ptr<BaseVector>>& base) -> bool { return GoddessCreateAction(base); };
	ItemCreateFunctions[Special_Item_Id::Cola]			= [&](vector<unique_ptr<BaseVector>>& base) -> bool { return ColaCreateAction(base); };
	ItemCreateFunctions[Special_Item_Id::Famicom]		= [&](vector<unique_ptr<BaseVector>>& base) -> bool { return FamicomCreateAction(base); };
	ItemCreateFunctions[Special_Item_Id::Mr_Nakamoto]	= [&](vector<unique_ptr<BaseVector>>& base) -> bool { return Mr_NakamotoCreateAction(base); };
	ItemCreateFunctions[Special_Item_Id::Dezeniman]		= [&](vector<unique_ptr<BaseVector>>& base) -> bool { return DezenimanCreateAction(base); };
}

//各アイテムの生成条件関数

//Bパネル : 敵を一匹も倒さずに扉の上を通過すると出現する隠れキャラ
bool CSpecial_Item_Manager::B_PanelCreateAction(vector<unique_ptr<BaseVector>>& base)
{
	//敵を倒していたらfalse
	if (gKillEnemyNum > 0) return false;

	//プレイヤーとドア(ゴール)を取得
	CPlayer* player = (CPlayer*)Get_obj(base, PLAYER);
	CDoor* door = (CDoor*)Get_obj(base, GOAL);

	if (!IsPlayer_match_Goal) {
		//プレイヤーの座標とドアの座標が一致している場合
		if ((player->SystemPos.x == door->SystemPos.x) &&
			(player->SystemPos.y == door->SystemPos.y))
		{
			IsPlayer_match_Goal = true; //プレイヤーとドアの座標一致フラグをtrue
			return false; //一旦falseで返す
		}
	}
	else
	{
		//プレイヤーの座標のx,yどちらかがドアの座標ではない場合
		if ((player->SystemPos.x != door->SystemPos.x) ||
			(player->SystemPos.y != door->SystemPos.y))
		{
			return true;
		}
	}

	return false;
}
//ゴーデス : 敵を全滅させた後、ステージの外周を回ると出現する隠れキャラ
bool CSpecial_Item_Manager::GoddessCreateAction(vector<unique_ptr<BaseVector>>& base)
{
	//敵が全滅していない場合はfalse
	if (gEnemySum != gKillEnemyNum)
		return false;

	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);

	//外周判定
	if ((p->SystemPos.x == left || p->SystemPos.x == right)  && (p->SystemPos.y >= top  && p->SystemPos.y <= bottom) ||
		(p->SystemPos.y == top  || p->SystemPos.y == bottom) && (p->SystemPos.x >= left && p->SystemPos.x <= right))
	{
		int key = p->SystemPos.y * MAP_CHIP_W + p->SystemPos.x;
		PassedOuterTiles.insert(key); //踏破を記録
	}
	//中央に入った場合
	else
		PassedOuterTiles.clear(); //リセット

	//踏破したタイル数が外周のタイル数以上になった場合
	if (PassedOuterTiles.size() >= OuterRoute.size())
	{
		return true;
	}

	return false;
}
//コーラ : 敵を全滅させる前に扉の上を通過し、そのまま同じ方向に進み続けていると出現する隠れキャラ
bool CSpecial_Item_Manager::ColaCreateAction(vector<unique_ptr<BaseVector>>& base)
{
	//敵を全滅させていたらfalse
	if (gEnemySum == gKillEnemyNum)
		return false;

	//プレイヤーとドア(ゴール)を取得
	CPlayer* player = (CPlayer*)Get_obj(base, PLAYER);
	CDoor* door = (CDoor*)Get_obj(base, GOAL);

	//移動していない場合はfalse
	if (player->Move_Dir == Move_Id::NONE_KEY) return false;

	if (!IsPlayer_match_Goal) {
		//プレイヤーの座標とドアの座標が一致している場合
		if ((player->SystemPos.x == door->SystemPos.x) &&
			(player->SystemPos.y == door->SystemPos.y))
		{
			IsPlayer_match_Goal = true; //プレイヤーとドアの座標一致フラグをtrue
			PrevMove_Dir = player->Move_Dir; //プレイヤーの移動方向を保存
			MoveCnt = 0;
			return false; //一旦falseで返す
		}
	}
	else
	{
		//ドア通過 : プレイヤーの座標のx,yどちらかがドアの座標ではない場合
		if ((player->SystemPos.x != door->SystemPos.x) ||
			(player->SystemPos.y != door->SystemPos.y))
		{
			if (player->Move_Dir != PrevMove_Dir)
			{
				IsPlayer_match_Goal = false; //フラグリセット
				MoveCnt = 0;
				return false;
			}
			else
				MoveCnt++;

			//17秒を超えた場合はtrue
			if (MoveCnt > 60 * 17)
				return true;
		}
	}

	return false;
}
//ファミコン : 敵を全滅させた後、爆弾を256個爆発させると出現する隠れキャラ
bool CSpecial_Item_Manager::FamicomCreateAction(vector<unique_ptr<BaseVector>>& base)
{
	//敵全滅の場合
	if (gEnemySum == gKillEnemyNum)
	{
		//爆弾を爆発させた個数が 256 以上になった場合
		if (gBombExplosionNum >= 256)
			return true;
	}

	return false;
}
//中本さん : 壁を爆破せずに敵を全滅させると出現する隠れキャラ
bool CSpecial_Item_Manager::Mr_NakamotoCreateAction(vector<unique_ptr<BaseVector>>& base)
{
	//一つでも破壊可能ブロックを爆破していた場合はfalse
	if (gIsExplosionBlock) return false;

	//敵を全滅させている場合true
	if (gEnemySum == gKillEnemyNum)
		return true;

	return false;
}
//デゼニマン : 敵を倒さずに壁をすべて破壊し、さらに扉に爆風を三回当てると出現する隠れキャラ
bool CSpecial_Item_Manager::DezenimanCreateAction(vector<unique_ptr<BaseVector>>& base)
{
	//敵を倒している場合はfalse
	if (gKillEnemyNum > 0) return false;

	//ドアを取得
	CDoor* door = (CDoor*)Get_obj(base, GOAL);

	if (!IsAllExplosionCrashBlock) 
	{
		for (int y = 0; y < MAP_CHIP_H; y++) {
			for (int x = 0; x < MAP_CHIP_W; x++) {

				//破壊可能ブロックがある場合はfalseで終了
				if (gNowMap[y][x] == Obj_Id::CRASH_BLOCK)
					return false;
			}
		}
		IsAllExplosionCrashBlock = true; //破壊可能ブロック全て破壊フラグtrue
		door->DoorExplosionNum = 0; //ドア爆破カウントリセット
	}
	
	//ドア爆破回数が3以上になった場合
	if (door->DoorExplosionNum >= 3)
		return true;

	return false;
}