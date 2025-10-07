#pragma once

#include"objBase.h"

class CPlayer : public BaseVector
{
public:

	//プレイヤーのアニメーションフレーム数
	static constexpr auto PLAYER_ANIM_FRAME = 8;

	//プレイヤーの画像の数
	static constexpr auto PLAYER_IMG_NUM = 12;

	//プレイヤーの一つのアニメーションに使用する数(配列の添え字にするように-1)
	static constexpr auto PLAYER_ONE_ANIM_NUM = 2;


	enum AnimMaxId
	{
		LEFT = 2,
		RIGHT = 5,
		UP = 8,
		DOWN = 11,
	};

public:

	CPlayer();
	~CPlayer();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//プレイヤーの移動処理
	void PlayerMove();

	//プレイヤーのアニメーション処理
	void PlayerAnim(AnimMaxId, int*);

	//爆弾を置く処理
	void PutExplosion(vector<unique_ptr<BaseVector>>&);

private:

	//プレイヤーの画像ハンドル配列
	int PlayerImgHandle[PLAYER_IMG_NUM];
	//アニメーションの画像インデックス
	int AnimIndex{ 0 };
	//アニメーションカウント
	int AnimCnt{ PLAYER_ANIM_FRAME };
	//直前のアニメーションID保存用
	AnimMaxId KeepAnimMaxId{ AnimMaxId::LEFT };

};