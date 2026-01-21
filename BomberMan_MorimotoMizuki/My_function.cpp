#include"My_function.h"

//文字列分割関数 : 指定した文字で文字列を分割する
//(文字列, 分割文字)
vector<string> split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;

	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

//キー入力チェック関数
bool Key_Check(Move_Id move_id)
{
	//パッドの状態の取得
	int pad = GetJoypadInputState(DX_INPUT_PAD1);
	switch (move_id)
	{
	case UP:
		if (CheckHitKey(KEY_INPUT_UP) || (pad & PAD_INPUT_UP))
			return true;

		break;
	case RIGHT:
		if (CheckHitKey(KEY_INPUT_RIGHT) || (pad & PAD_INPUT_RIGHT))
			return true;

		break;
	case DOWN:
		if (CheckHitKey(KEY_INPUT_DOWN) || (pad & PAD_INPUT_DOWN))
			return true;

		break;
	case LEFT:
		if (CheckHitKey(KEY_INPUT_LEFT) || (pad & PAD_INPUT_LEFT))
			return true;

		break;
	case LEFT_CLICK:
		if ((GetMouseInput() & MOUSE_INPUT_LEFT))
			return true;

		break;
	case SPACE:
		if (CheckHitKey(KEY_INPUT_SPACE))
			return true;

		break;
	case ENTER:
		if (CheckHitKey(KEY_INPUT_RETURN))
			return true;

		break;
	case B_KEY:
		if (CheckHitKey(KEY_INPUT_B))
			return true;
		break;
	case T_KEY:
		if (CheckHitKey(KEY_INPUT_T))
			return true;
		break;
	case TAB_KEY:
		if (CheckHitKey(KEY_INPUT_TAB))
			return true;
		break;
	case ALL_KEY:
		if (CheckHitKeyAll() || pad)
			return true;

		break;
	}
	return false;
}

//円と矩形判定処理(円オブジェクト,矩形オブジェクト,円の半径)　脇坂先生のやつに distance のずれを追加
void HitCheck_Box_Circle(BaseVector* a, BaseVector* b, float radius, float distance) {
	Point pc{ a->pos.x + a->vec.x + a->ImgWidth / 2,a->pos.y + a->vec.y + a->ImgHeight / 2 };//中心座標計算
	Point n{ -1,-1 };//矩形の一番近い座標
	n.x = Circle_Center_With_Closest_Point(pc.x, b->pos.x - distance, b->pos.x + b->ImgWidth - distance);
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

//プレイヤーのステータスをリセット
void ResetPlayerStatus()
{
	switch (gGamePhase)
	{
	case GAMEOVER:
		gPlayerStatus.bombLevel				= 1;				//爆弾の火力
		gPlayerStatus.bombPutNum			= 1;				//爆弾の設置可能数
		gPlayerStatus.speed					= PLAYER_SPEED;		//プレイヤーのスピード
		gPlayerStatus.isBombPass			= false;			//↓アイテム能力 --------
		gPlayerStatus.isFlameBarrier		= false;
		gPlayerStatus.isPerfectMan			= false;
		gPlayerStatus.isRemoteController	= false;
		gPlayerStatus.isWallPass			= false;			//-----------------------
		break;
	case GAMECLEAR:
		break;
	}

	gKillEnemyNum	= 0;	//倒した敵の数
	gNowBombNum		= 0;	//設置した爆弾の数
	gBombId			= 0;	//爆弾のID
	gBombExplosionNum = 0;	//爆弾を爆発させた個数
	gIsExplosionBlock = false;	//破壊可能ブロックの爆破フラグ
}

//プレイヤーのステータス最強
void SuperPlayerStatus()
{
	gPlayerStatus.bombPutNum				= gSuperPlayerStatus.bombPutNum;
	gPlayerStatus.bombLevel					= gSuperPlayerStatus.bombLevel;
	gPlayerStatus.speed						= gSuperPlayerStatus.speed;
	gPlayerStatus.isBombPass				= gSuperPlayerStatus.isBombPass;
	gPlayerStatus.isFlameBarrier			= gSuperPlayerStatus.isFlameBarrier;
	gPlayerStatus.isPerfectMan				= gSuperPlayerStatus.isPerfectMan;
	gPlayerStatus.isRemoteController		= gSuperPlayerStatus.isRemoteController;
	gPlayerStatus.isWallPass				= gSuperPlayerStatus.isWallPass;
}

//サウンド再生関数(MusicVolume : 音量0～255)
void My_PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag, int MusicVolume)
{
	PlaySoundMem(SoundHandle, PlayType, TopPositionFlag);	//サウンド再生
	ChangeVolumeSoundMem(MusicVolume, SoundHandle);			//音量調整  音量0～255
}