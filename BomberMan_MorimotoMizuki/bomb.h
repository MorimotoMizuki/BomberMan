#pragma once

#include"objBase.h"

class CBomb : public BaseVector
{
public:

	//爆弾のアニメーションフレーム数
	static constexpr auto BOMB_ANIM_FRAME = 16;

	//爆弾画像の数
	static constexpr auto BOMB_IMG_NUM = 3;

	//爆弾のアニメーション数
	static constexpr auto BOMB_ANIM_NUM = 4;

	//爆弾アニメーションの順番
	static constexpr int BOMB_ANIM_ORDER[BOMB_ANIM_NUM] = { 0,1,2,1 };

public:

	CBomb(Point, int = 180);
	~CBomb();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//爆弾アニメーション処理
	void BombAnim(int, int*);

	//爆発エフェクト
	void ExplosionEffect(vector<unique_ptr<BaseVector>>&);

private:

	//爆弾画像ハンドル配列
	int BombImgHandle[BOMB_IMG_NUM];
	//アニメーションの画像インデックス
	int AnimIndex{ 0 };
	//アニメーションカウント
	int AnimCnt{ BOMB_ANIM_FRAME };

	//生存時間(フレーム数)
	int SurvivalFrame{ 0 };
	//死亡時間(フレーム数)
	int DiedFrame{ 0 };
};