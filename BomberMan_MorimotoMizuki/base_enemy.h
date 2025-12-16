#pragma once

#include"objBase.h"
#include"astar.h"
#include<list>

class CBaseEnemy : public BaseVector
{
public:

	//左右上下
	static constexpr MapPoint ADD_VEC[4] = {
		{-1,0},
		{1,0},
		{0,-1},
		{0,1}
	};
	//方向ID
	enum MoveDir
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};

public:

	CBaseEnemy() = default;
	virtual ~CBaseEnemy() = default;

	//コンストラクタ : 画像ハンドル、システム上の座標
	void Constructor(Point, MapPoint);

	//デストラクタ
	void Destructor();

	//敵の死亡時処理
	void EnemyDead(int ANIM_FRAME, int deadAnimFrame);

	//爆弾と接触時の座標調整処理
	void HitBomb_PosAdjustment(vector<unique_ptr<BaseVector>>&);

	//アニメーション処理
	bool Anim(int ANIM_FRAME, int animMax, int* index, bool loop);

	//ランダム移動処理
	void RandomMove(vector<unique_ptr<BaseVector>>&);
	void SetMoveDir(vector<unique_ptr<BaseVector>>&);

	//プレイヤー追跡処理
	void TrackingPlayerMove(CPlayer* p, int moveFrame, bool* isTrackingPlayer);

	//敵の死亡時のパラメータ設定
	virtual void SetEnemyDeadParameter() = 0;

	//スコア表示処理
	void DrawScore();

public:
	
	float SPEED = 2.0f;	//移動速度

	int SCORE = 0; //スコア

protected:

	//敵の死亡時画像
	int EnemyDeadImgHandle[5]{ 0,0,0,0,0 };

	//プレイヤーとの差分
	float Distance{ 0.0f };
	//死亡フラグ
	bool IsDead{ false };
	//死亡カウント
	int DeadCnt{ 0 };

	//アニメーションの画像インデックス
	int AnimIndex{ 0 };
	//アニメーションカウント
	int AnimCnt{ 0 };

	int StopCnt{ 0 };

	//プレイヤー追跡用
	vector<Cell> vec_last_route;
	int move_cnt{ 0 };

	//スコア表示フラグ
	bool IsDrawScore{ false };
};