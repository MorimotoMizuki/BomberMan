#pragma once

#include"objBase.h"

class CBaseItem : public BaseVector
{
public:

	CBaseItem() = default;
	virtual ~CBaseItem() = default;

	//コンストラクタ : 座標、システム上の座標
	void Constructor(Point, MapPoint);

	//デストラクタ
	void Destructor();

	//アイテム削除
	void DeleteItem();

	//アイテムの共通更新処理
	int ItemAction(vector<unique_ptr<BaseVector>>& base);

	//アイテム獲得時処理
	virtual void ItemGetAction() = 0;

	//アイテム爆破時処理
	virtual void ItemExplosionAction(vector<unique_ptr<BaseVector>>& base) = 0;

	//各アイテムの関数設定
	virtual void SetItemFunction() = 0;

public:

	int Score = 0; //スコア

	//プレイヤーとの差分
	float Distance{ 0.0f };

	//アイテム爆破フラグ
	bool IsItemExplosion{ false };

protected:

	//アイテム獲得SE
	int SE_ItemGet{ -1 };

	//爆破カウント
	int ExplosionCnt{ 0 };

};