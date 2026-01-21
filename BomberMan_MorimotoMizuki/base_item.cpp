#include"obj.h"
#include"function.h"

//コンストラクタ
void CBaseItem::Constructor(Point p, MapPoint system_p)
{
	SE_ItemGet = LoadSoundMem("sound\\ItemGet_SE.wav");
	pos = p;

	SystemPos = system_p;

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	ID = Obj_Id::ITEM;
	pri = Pri_Id::pITEM;
}

//デストラクタ
void CBaseItem::Destructor()
{
	DeleteSoundMem(SE_ItemGet);
}

//アイテムの共通更新処理
int CBaseItem::ItemAction(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return 0;

	Distance = p->Distance; //プレイヤーの画面の差分を取得

	//アイテムの削除処理
	if (!draw_flag) {
		if (!CheckSoundMem(SE_ItemGet)) {
			FLAG = false;
		}
		return 0;
	}

	//アイテム獲得処理
	if (p->SystemPos.x == SystemPos.x && p->SystemPos.y == SystemPos.y)
	{
		if (gNowMap[SystemPos.y][SystemPos.x] == Obj_Id::NONE)
			ItemGetAction();
	}

	//アイテムの爆破 + 遅延処理
	if (IsItemExplosion) {
		ExplosionCnt++;
		if (ExplosionCnt > 60) {

			ItemExplosionAction(base);
			IsItemExplosion = false;
			ExplosionCnt = 0;
		}
	}
}

//アイテム削除
void CBaseItem::DeleteItem()
{
	//SE再生
	My_PlaySoundMem(SE_ItemGet, DX_PLAYTYPE_BACK, TRUE, MusicVolume::SE_ItemGet);

	draw_flag = false;
	gPlayerStatus.score += Score; //スコア加算
}