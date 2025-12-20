#pragma once

#include"objBase.h"
#include"astar.h"
#include<list>
#include<array>
#include <utility>
#include<vector>

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
	void RandomMove(vector<unique_ptr<BaseVector>>&, const std::array<bool, 4>&, int change_dir_percentage);
	void SetMoveDir(vector<unique_ptr<BaseVector>>&, const std::array<bool, 4>&);

	//直線方向移動のフラグ変更処理
	int SetLineMoveIsDir(int dir_change_cnt, int STOP_FRAME);

	//プレイヤー追跡処理 : (プレイヤー情報、移動フレーム、プレイヤー追跡中フラグ、追跡パラメータ設定(距離制限をつけるかフラグ、追跡距離))
	void TrackingPlayerMove(CPlayer* p, float moveFrame, bool* isTrackingPlayer, std::pair<bool, int> tracking_parameter, int randomParameter);

	//敵の死亡時のパラメータ設定
	virtual void SetEnemyDeadParameter() = 0;

	//スコア表示処理
	void DrawScore();

	//死亡フラグ取得
	bool GetIsDead() { return IsDead; };

	//移動できるマスか判定する
	bool CheckMoveArea(MapPoint system_pos, std::vector<Obj_Id> hit_objId);

	//マップ範囲内か判定する
	bool CheckOutsideRangeArea(MapPoint system_pos);

	//設定したObj_Idか判定する
	bool CheckArea_of_SelectObj_Id(Obj_Id obj_id, std::initializer_list<Obj_Id> obj_id_array);

public:
	
	float SPEED = 2.0f;	//移動速度

	int SCORE = 0; //スコア

	int STOP_FRAME = 0; //停止フレーム

protected:

	//敵の死亡時画像
	int EnemyDeadImgHandle[5]{ 0,0,0,0,0 };

	//移動制限判定Obj_Id
	std::vector<Obj_Id> HitMoveObj_Id;

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
	float move_cnt{ 0.0f };

	//スコア表示フラグ
	bool IsDrawScore{ false };

	//移動方向許可フラグ配列
	std::array<bool, 4> IsPermitDir{ true,true,true,true };
};