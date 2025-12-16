#pragma once

#include"objBase.h"
#include"base_enemy.h"

//コンドリア
class CKondoria : public CBaseEnemy
{
public:

	//アニメーションフレーム数
	static constexpr auto KONDORIA_ANIM_FRAME = 0;

	//画像の数
	static constexpr auto KONDORIA_IMG_NUM = 4;

	//アニメーション数
	static constexpr auto KONDORIA_ANIM_NUM = 0;

public:

	//画像ハンドル、システム上の座標
	CKondoria(Point, MapPoint);
	~CKondoria();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//敵の死亡時のパラメータ設定
	void SetEnemyDeadParameter() override;

private:

	//画像ハンドル
	int ImgHandle[KONDORIA_IMG_NUM]{ 0,0,0,0 };
};