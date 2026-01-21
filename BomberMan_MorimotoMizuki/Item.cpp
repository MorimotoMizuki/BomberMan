#include"obj.h"
#include"function.h"

CNormalItem::CNormalItem(Point p, MapPoint system_p, Item_Id item_id)
{
	CBaseItem::Constructor(p, system_p); //ベースのコンストラクタ

	LoadDivGraph("image\\Item.png", ITEM_IMG_NUM, 8, 1, IMGSIZE16, IMGSIZE16, ItemImgHandle);

	SetItemFunction(); //各アイテムの関数設定

	ItemID = item_id;

	Score = 1000;
}

int CNormalItem::Action(vector<unique_ptr<BaseVector>>& base)
{
	CBaseItem::ItemAction(base); //ベースの更新処理

	return 0;
}

void CNormalItem::Draw()
{
	if (!draw_flag) return;

	//画像描画
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, ItemImgHandle[ItemID], true);
}

CNormalItem::~CNormalItem()
{
	CBaseItem::Destructor(); //ベースのデストラクタ

	for (int i = 0; i < ITEM_IMG_NUM; i++)
		DeleteGraph(ItemImgHandle[i]);
}

//アイテム獲得時処理
void CNormalItem::ItemGetAction()
{
	if (ItemFunctions.contains(ItemID))
		ItemFunctions[ItemID]();   // 対応する関数を呼び出す
}

//アイテム爆破時処理
void CNormalItem::ItemExplosionAction(vector<unique_ptr<BaseVector>>& base)
{
	Point p{ SystemPos.x * CHIP_SIZE, SystemPos.y * CHIP_SIZE + WINDOW_HEADER };

	for (int i = 0; i < 4; i++) {

		switch (ItemID)
		{
		case FirePower:
			//オニール生成
			base.emplace_back((unique_ptr<BaseVector>) new COneal(p, SystemPos));
			break;
		case Bomb:
			//バロム生成
			base.emplace_back((unique_ptr<BaseVector>) new CBallom(p, SystemPos));
			break;
		case RemoteController:
			//コンドリア生成
			base.emplace_back((unique_ptr<BaseVector>) new CKondoria(p, SystemPos));
			break;
		case Boots:
			//ダル生成
			base.emplace_back((unique_ptr<BaseVector>) new CDahl(p, SystemPos));
			break;
		case BombPassing:
			//オバピー生成
			base.emplace_back((unique_ptr<BaseVector>) new COvapee(p, SystemPos));
			break;
		case WallPassing:
			//ミンボー生成
			base.emplace_back((unique_ptr<BaseVector>) new CMinvo(p, SystemPos));
			break;
		case FlameBarrier:
			//バース生成
			base.emplace_back((unique_ptr<BaseVector>) new CPass(p, SystemPos));
			break;
		case PerfectMan:
			//ポンタン生成
			base.emplace_back((unique_ptr<BaseVector>) new CKondoria(p, SystemPos));
			break;
		}
	}

	FLAG = false; //アイテム削除
}

//各アイテムの関数設定
void CNormalItem::SetItemFunction()
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
void CNormalItem::FirePowerAction()
{
	if(gPlayerStatus.bombLevel < MAX_FIRE_POWER)
		gPlayerStatus.bombLevel++;

	CBaseItem::DeleteItem();
}
//爆弾の置ける数アップ			: バロム
void CNormalItem::BombAction()
{
	if(gPlayerStatus.bombPutNum < MAX_PUT_BOMB_NUM)
		gPlayerStatus.bombPutNum++;
	CBaseItem::DeleteItem();
}
//爆弾がBボタンで自由に爆発可能	: コンドリア
void CNormalItem::RemoteControllerAction()
{
	gPlayerStatus.isRemoteController = true;
	CBaseItem::DeleteItem();
}
//移動速度アップ				: ダル
void CNormalItem::BootsAction()
{
	if (gPlayerStatus.speed < MAX_SPEED)
		gPlayerStatus.speed += PLAYER_SPEED / 2;
	CBaseItem::DeleteItem();
}
//爆弾の上を歩けるようになる	: オバピー
void CNormalItem::BombPassingAction()
{
	gPlayerStatus.isBombPass = true;
	CBaseItem::DeleteItem();
}
//壁の上を歩けるようになる		: ミンボー
void CNormalItem::WallPassingAction()
{
	gPlayerStatus.isWallPass = true;
	CBaseItem::DeleteItem();
}
//爆風で死ななくなる			: バース
void CNormalItem::FlameBarrierAction()
{
	gPlayerStatus.isFlameBarrier = true;
	CBaseItem::DeleteItem();
}
//30秒間無敵になる				: ポンタン
void CNormalItem::PerfectManAction()
{
	gPlayerStatus.isPerfectMan = true;
	CBaseItem::DeleteItem();
}