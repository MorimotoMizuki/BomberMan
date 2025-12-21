#include"obj.h"
#include"function.h"

CItem::CItem(Point p, MapPoint system_p, Item_Id item_id)
{
	LoadDivGraph("image\\Item.png", ITEM_IMG_NUM, 8, 1, IMGSIZE16, IMGSIZE16, ItemImgHandle);

	pos = p;

	SystemPos = system_p;

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	ItemID = item_id;

	SetItemFunction(); //各アイテムの関数設定

	ID  = Obj_Id::ITEM;
	pri = Pri_Id::pITEM;
}

int CItem::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return 0;

	Distance = p->Distance; //プレイヤーの画面の差分を取得

	if (p->SystemPos.x == SystemPos.x && p->SystemPos.y == SystemPos.y)
	{
		if (ItemFunctions.contains(ItemID))
			ItemFunctions[ItemID]();   // 対応する関数を呼び出す
	}

	return 0;
}

void CItem::Draw()
{
	//画像描画
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, ItemImgHandle[ItemID], true);
}

CItem::~CItem()
{
	for (int i = 0; i < ITEM_IMG_NUM; i++)
		DeleteGraph(ItemImgHandle[i]);
}

//アイテム削除
void CItem::DeleteItem()
{
	gPlayerStatus.score += 1000; //スコア加算
	FLAG = false;
}

//各アイテムの関数設定
void CItem::SetItemFunction()
{
	ItemFunctions[FirePower]		= [&]() {FirePowerAction(); };
	ItemFunctions[Bomb]				= [&]() {BombAction(); };
	ItemFunctions[RemoteController] = [&]() {RemoteControllerAction(); };
	ItemFunctions[Boots]			= [&]() {BootsAction(); };
	ItemFunctions[BombPassing]		= [&]() {BombPassingAction(); };
	ItemFunctions[WallPassing]		= [&]() {WallPassingAction(); };
	ItemFunctions[FlameBarrier]		= [&]() {FlameBarrierAction(); };
	ItemFunctions[PerfectMan]		= [&]() {PerfectManAction(); };
}

//火力アップ					: オニール
void CItem::FirePowerAction()
{
	if(gPlayerStatus.bombLevel < MAX_FIRE_POWER)
		gPlayerStatus.bombLevel++;

	DeleteItem();
}
//爆弾の置ける数アップ			: バロム
void CItem::BombAction()
{
	if(gPlayerStatus.bombPutNum < MAX_PUT_BOMB_NUM)
		gPlayerStatus.bombPutNum++;
	DeleteItem();
}
//爆弾がBボタンで自由に爆発可能	: コンドリア
void CItem::RemoteControllerAction()
{
	gPlayerStatus.isRemoteController = true;
	DeleteItem();
}
//移動速度アップ				: ダル
void CItem::BootsAction()
{
	gPlayerStatus.speed += PLAYER_SPEED / 2;
	DeleteItem();
}
//爆弾の上を歩けるようになる	: オバピー
void CItem::BombPassingAction()
{
	gPlayerStatus.isBombPass = true;
	DeleteItem();
}
//壁の上を歩けるようになる		: ミンボー
void CItem::WallPassingAction()
{
	gPlayerStatus.isWallPass = true;
	DeleteItem();
}
//爆風で死ななくなる			: バース
void CItem::FlameBarrierAction()
{
	gPlayerStatus.isFlameBarrier = true;
	DeleteItem();
}
//30秒間無敵になる				: ポンタン
void CItem::PerfectManAction()
{
	gPlayerStatus.isPerfectMan = true;
	DeleteItem();
}