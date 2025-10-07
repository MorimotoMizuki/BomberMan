#pragma once
#include "Scene_Manager.h"

class CManager;//シーン管理クラス

//シーンベースクラス
class CScene {
protected:
	CManager* manager{ nullptr };
public:
	CScene(CManager* pManager) { manager = pManager; };

	//更新処理
	virtual int Update() { return 0; };
	//描画処理
	virtual void Draw() {};
	//デストラクタ
	virtual ~CScene() {};
};