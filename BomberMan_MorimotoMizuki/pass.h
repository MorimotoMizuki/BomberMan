#pragma once

#include"objBase.h"
#include"base_enemy.h"

//バース
class CPass : public CBaseEnemy
{
public:

	//アニメーションフレーム数
	static constexpr auto PASS_ANIM_FRAME = 8;

	//画像の数
	static constexpr auto PASS_IMG_NUM = 4;

	//アニメーション数
	static constexpr auto PASS_ANIM_NUM = 4;

public:

	//画像ハンドル、システム上の座標
	CPass(Point, MapPoint);
	~CPass();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

private:

	//画像ハンドル
	int ImgHandle[PASS_IMG_NUM]{ 0,0,0,0 };

	//プレイヤー追跡フラグ
	bool isTrackingPlayer{ false };

	//プレイヤー追跡パラメータ
	std::pair<bool, int> TrackingParameter = std::make_pair<bool, int>(false, 2);

	//プレイヤーの座標のXYのどちらかがオブジェクトと同じ座標フラグ
	bool IsPlayerSamePosXY{ false };
	// ↑ のフラグリセット用
	int ResetSamePosXYCnt{ 0 };

	int TrackingPlayerPercentage{ 0 };
};