#pragma once

#include"objBase.h"

class CExplosion : public BaseVector
{
public:

	//爆発の削除フレーム数
	static constexpr auto EXPLOSION_DELETE_FRAME = 21;

	//爆発のアニメーション数
	static constexpr auto EXPLOSION_ANIM_NUM = 7;

	//爆発のアニメーションフレーム数
	static constexpr auto EXPLOSION_ANIM_FRAME = EXPLOSION_DELETE_FRAME / EXPLOSION_ANIM_NUM;

	//爆発の画像の数
	static constexpr auto EXPLOSION_IMG_NUM = 12;

	//爆発アニメーションの順番
	static constexpr int EXPLOSION_ANIM_ORDER[EXPLOSION_ANIM_NUM] = { 0,1,2,3,2,1,0 };

	enum ExplosionDir{
		UP_exp,
		DOWN_exp,
		RIGHT_exp,
		LEFT_exp,
	};

public:

	//描画座標,システム上の座標 爆弾レベル
	CExplosion(Point, MapPoint, int);
	~CExplosion();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//爆発アニメーション処理
	bool ExplosionAnim(int animMax, int* index);

	//爆弾描画(レベル別)
	//ずれ分の座標x、y、描画個数、縦or横
	std::tuple<Point, Point, int> DrawExplosion(float, float, int, ExplosionEffectId);

	//当たり判定
	void HitAction(vector<unique_ptr<BaseVector>>&);

private:

	//処理終了フラグ
	bool IsEnd{ false };

	//爆弾レベル
	int	BombLevel;

	//爆発の画像ハンドル配列
	int ExplosionImgHandle[EXPLOSION_IMG_NUM];
	//アニメーションの画像インデックス
	int AnimIndex{ 0 };
	//アニメーションカウント
	int AnimCnt{ 0 };

	//プレイヤーとの差分
	float Distance{ 0.0f };

	//爆弾の座標
	MapPoint BombPos{ 0,0 };

	//本体からのずれ座標とカウントのデータ
	//(本体からのずれ座標, 爆発方向, 爆発する数)
	std::vector<std::tuple<Point, Point, int>> ExplosionPointData = {
		{ {0, 0},{0, 0}, 0 },
		{ {0, 0},{0, 0}, 0 },
		{ {0, 0},{0, 0}, 0 },
		{ {0, 0},{0, 0}, 0 }
	};

	//クラッシュブロックのシステム上の座標
	MapPoint CrashBlockPos[4]{
		{0,0},
		{0,0},
		{0,0},
		{0,0},
	};

	//サウンド用
	int SE_BombExplosion{ -1 };

	//初回のみ当たり判定フラグ
	bool IsHitOnce{ false };

	//敵を同時に倒した数
	int ConcurrentKillEnemyNum{ 0 };
};