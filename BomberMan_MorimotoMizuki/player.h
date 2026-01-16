#pragma once

#include"objBase.h"
#include<stack>

class CPlayer : public BaseVector
{
public:

	//プレイヤーのアニメーションフレーム数
	static constexpr auto PLAYER_ANIM_FRAME = 8;

	//プレイヤーの画像の数
	static constexpr auto PLAYER_IMG_NUM = 18;

	//プレイヤーの一つのアニメーションに使用する数(配列の添え字にするように-1)
	static constexpr auto PLAYER_ONE_ANIM_NUM = 2;

	enum AnimMaxId
	{
		LEFT = 2,
		RIGHT = 5,
		UP = 8,
		DOWN = 11,
		DEAD = 17,
	};

	enum PlayerStateId
	{
		NONEplayer,
		PLAYplayer,
		DEADplayer,
	};

public:

	CPlayer();
	~CPlayer();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//プレイヤーの移動処理
	void PlayerMove();

	//プレイヤーの当たり判定
	void PlayerHit(vector<unique_ptr<BaseVector>>&);

	//プレイヤーのアニメーション処理
	//(アニメーション最大数、アニメーションフレーム、インデックス*、繰り返しフラグ)
	bool PlayerAnim(AnimMaxId, int, int*, bool = true);

	//爆弾を置く処理
	void PutExplosion(vector<unique_ptr<BaseVector>>&);

	//プレイヤーの死亡時処理
	void PlayerDead();

	//プレイヤー死亡時の値設定
	void SetPlayerDead(PlayerStateId state) { 
		PlayerState = state; 
		AnimIndex = 12;
		AnimCnt = PLAYER_ANIM_FRAME;
	}

	//安全な座標かチェックする
	bool IsValidMapPos(MapPoint p);

	//リモコンの処理
	void RemoteControllerAction(vector<unique_ptr<BaseVector>>&);

	//プレイヤーの移動時のSE再生関数
	void PlayerMoveSound(int play_sound_handle, int stop_sound_handle, int sound_volume);

public:

	//マップの位置
	Point m_pos{ 0,0 };

	//m_pos と pos の差分
	float Distance{ 0.0f };

	//爆弾設置処理
	bool IsPutBomb{ false };

	//リモコン使用時の爆弾のスタック
	std::vector<int> BombStack;

private:

	//プレイヤーの画像ハンドル配列
	int PlayerImgHandle[PLAYER_IMG_NUM];
	//アニメーションの画像インデックス
	int AnimIndex{ 0 };
	//アニメーションカウント
	int AnimCnt{ PLAYER_ANIM_FRAME };
	//直前のアニメーションID保存用
	AnimMaxId KeepAnimMaxId{ AnimMaxId::LEFT };

	//プレイヤーの状態
	PlayerStateId PlayerState{ PlayerStateId::PLAYplayer };

	//キー保存用
	bool KeyCheck[2]{ false, false };

	//パーフェクトマン用のカウント
	int PerfectManCnt{ 0 };

	//サウンド
	int SE_PutBomb{ -1 };	//爆弾配置時
	int SE_PlayerDeadStart{ -1 }; //プレイヤーが死んだ直後に再生するSE
	int SE_PlayerDead{ -1 };	//プレイヤーが死んだ時に再生するSE

	int SE_PlayerWalk_W{ -1 };	//プレイヤーが横方向に移動する時のSE
	int SE_PlayerWalk_H{ -1 }; //プレイヤーが縦方向に移動する時のSE

	bool IsPlayerDeadStartSE{ false }; //死んだ直後のSEの再生フラグ
};