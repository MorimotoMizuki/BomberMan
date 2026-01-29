#pragma once

#include"objBase.h"
#include"base_enemy.h"

//バロム
class CBallom : public CBaseEnemy
{
private:

	//バロムのアニメーションフレーム数
	static constexpr auto BALLOM_ANIM_FRAME = 8;

	//バロムの画像の数
	static constexpr auto BALLOM_IMG_NUM = 4;

	//バロムのアニメーション数
	static constexpr auto BALLOM_ANIM_NUM = 4;

	//バロムアニメーションの順番
	static constexpr int BALLOM_ANIM_ORDER[BALLOM_ANIM_NUM] = { 0,1,2,1 };

public:

	//画像ハンドル、システム上の座標
	CBallom(Point, MapPoint, bool is_door_exp = false);
	~CBallom();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

private:

	//画像ハンドル
	int BallomImgHandle[BALLOM_IMG_NUM]{ 0,0,0,0 };

	//ドア爆破で生成された敵フラグ
	bool IsDoorExplosionEnemy{ false };
};