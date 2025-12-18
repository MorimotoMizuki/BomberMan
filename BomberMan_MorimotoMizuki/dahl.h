#pragma once

#include"objBase.h"
#include"base_enemy.h"

//ダル
class CDahl : public CBaseEnemy
{
public:

	//アニメーションフレーム数
	static constexpr auto DAHL_ANIM_FRAME = 8;

	//画像の数
	static constexpr auto DAHL_IMG_NUM = 4;

	//アニメーション数
	static constexpr auto DAHL_ANIM_NUM = 4;

public:

	//画像ハンドル、システム上の座標
	CDahl(Point, MapPoint);
	~CDahl();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//敵の死亡時のパラメータ設定
	void SetEnemyDeadParameter() override;

private:

	//画像ハンドル
	int ImgHandle[DAHL_IMG_NUM]{ 0,0,0,0 };

	//方向変更カウント
	int DirChangeCnt{ 0 };
};