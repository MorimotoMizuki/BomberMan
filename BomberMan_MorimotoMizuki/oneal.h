#pragma once

#include"objBase.h"
#include"base_enemy.h"

//オニール
class COneal : public CBaseEnemy
{
public:

	//アニメーションフレーム数
	static constexpr auto ONEAL_ANIM_FRAME = 8;

	//画像の数
	static constexpr auto ONEAL_IMG_NUM = 4;

	//アニメーション数
	static constexpr auto ONEAL_ANIM_NUM = 4;

public:

	//画像ハンドル、システム上の座標
	COneal(Point, MapPoint);
	~COneal();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//敵の死亡時のパラメータ設定
	void SetEnemyDeadParameter() override;

private:

	//画像ハンドル
	int ImgHandle[ONEAL_IMG_NUM]{ 0,0,0,0 };

	bool isTrackingPlayer{ false };
};