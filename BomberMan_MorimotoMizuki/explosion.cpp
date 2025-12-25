#include"obj.h"
#include"function.h"
#include"map.h"

CExplosion::CExplosion(Point p, MapPoint bombP, int bombLevel)
{
	LoadDivGraph("image\\explosion.png", EXPLOSION_IMG_NUM, 4, 3, IMGSIZE32, IMGSIZE32, ExplosionImgHandle);

	SE_BombExplosion = LoadSoundMem("sound\\BombExplosion.wav");

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	//爆弾の座標
	BombPos = bombP;

	//座標を升目 * チップサイズ - 画面上のずれ　で計算
	pos.x = BombPos.x * CHIP_SIZE;
	pos.y = BombPos.y * CHIP_SIZE + WINDOW_HEADER;

	//爆弾レベル
	BombLevel = bombLevel;

	ID  = Obj_Id::EXPLOSION;
	pri = Pri_Id::pEXPLOSION;

	//SE再生
	PlaySoundMem(SE_BombExplosion, DX_PLAYTYPE_BACK);
}

int CExplosion::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if(p != nullptr)
		Distance = p->Distance;

	//当たり判定
	HitAction(base);

	//爆発アニメーション処理
	if (!ExplosionAnim(EXPLOSION_ANIM_NUM, &AnimIndex))
		FLAG = false;

	return 0;
}

void CExplosion::Draw()
{
	//画像描画
	//中央
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, ExplosionImgHandle[EXPLOSION_ANIM_ORDER[AnimIndex]], true);

	//上,下,左,右
	ExplosionPointData[0] = DrawExplosion( 0, -1	, BombLevel, ExplosionEffectId::VERTICAL);
	ExplosionPointData[1] = DrawExplosion( 0,  1	, BombLevel, ExplosionEffectId::VERTICAL);
	ExplosionPointData[2] = DrawExplosion(-1,  0	, BombLevel, ExplosionEffectId::HORIZONTAL);
	ExplosionPointData[3] = DrawExplosion( 1,  0	, BombLevel, ExplosionEffectId::HORIZONTAL);
}

CExplosion::~CExplosion()
{
	//現在設置している爆弾の数 : 減少
	gNowBombNum--;

	for (int i = 0; i < EXPLOSION_IMG_NUM; i++)
		DeleteGraph(ExplosionImgHandle[i]);

	DeleteSoundMem(SE_BombExplosion);
}

//当たり判定
void CExplosion::HitAction(vector<unique_ptr<BaseVector>>& base)
{
	if (std::get<0>(ExplosionPointData[0]).x == 0)
		return;

	//アニメーションインデックスが一定以上の場合は終了
	if (AnimIndex >= EXPLOSION_ANIM_NUM - 1 - 2)
		return; 

	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return;

	//クラッシュブロックとの判定
	for (int i = 0; i < 4; i++)
	{
		//爆弾の座標 + 方向
		MapPoint ex_pos{ BombPos.x + std::get<1>(ExplosionPointData[i]).x, BombPos.y + std::get<1>(ExplosionPointData[i]).y };

		//爆弾の威力分ループ
		for (int j = 0; j < std::get<2>(ExplosionPointData[i]); j++) {

			//クラッシュブロックの場合
			if (gNowMap[ex_pos.y][ex_pos.x] == 1) {
				//マップのデータ削除
				gNowMap[ex_pos.y][ex_pos.x] = Obj_Id::NONE;
				break;
			}
			//座標更新
			ex_pos.x += std::get<1>(ExplosionPointData[i]).x;
			ex_pos.y += std::get<1>(ExplosionPointData[i]).y;
		}
	}

	for (int i = 0; i < base.size(); i++)
	{
		//削除対象のオブジェクトはスキップ
		if (!base[i]->FLAG || !base[i]->draw_flag)
			continue;

		//敵との判定
		if (base[i]->ID == ENEMY)
		{
			MapPoint enemy_pos = base[i].get()->SystemPos;
			for (int k = 0; k < 4; k++)
			{
				//爆弾の座標 + 方向
				MapPoint ex_pos{ BombPos.x + std::get<1>(ExplosionPointData[k]).x, BombPos.y + std::get<1>(ExplosionPointData[k]).y };
				//爆弾の威力分ループ
				for (int j = 0; j < std::get<2>(ExplosionPointData[k]); j++) {

					//爆発の座標と敵の座標が一致した場合
					if (ex_pos.x == enemy_pos.x && ex_pos.y == enemy_pos.y) {

						((CBaseEnemy*)base[i].get())->SetEnemyDeadParameter();
						break;
					}
					//座標更新
					ex_pos.x += std::get<1>(ExplosionPointData[k]).x;
					ex_pos.y += std::get<1>(ExplosionPointData[k]).y;
				}
			}
		}

		//爆弾との判定 : 誘爆処理
		if (base[i]->ID == Obj_Id::BOMB) {
			MapPoint bomb_pos = base[i].get()->SystemPos;
			for (int k = 0; k < 4; k++)
			{
				//爆弾の座標 + 方向
				MapPoint ex_pos{ BombPos.x + std::get<1>(ExplosionPointData[k]).x, BombPos.y + std::get<1>(ExplosionPointData[k]).y };
				//爆弾の威力分ループ
				for (int j = 0; j < std::get<2>(ExplosionPointData[k]); j++) {

					//爆発の座標と敵の座標が一致した場合
					if (ex_pos.x == bomb_pos.x && ex_pos.y == bomb_pos.y) {

						((CBomb*)base[i].get())->ExplosionEffect(base, p);
						//爆弾のIDの要素をリストから削除
						auto it = std::find(p->BombStack.begin(), p->BombStack.end(), ((CBomb*)base[i].get())->GetBombID());
						if (it != p->BombStack.end())
							p->BombStack.erase(it);

						break;
					}
					//座標更新
					ex_pos.x += std::get<1>(ExplosionPointData[k]).x;
					ex_pos.y += std::get<1>(ExplosionPointData[k]).y;
				}
			}
		}
	}

	if (IsEnd) return;

	//火炎バリアフラグが true の場合は終了
	if (gPlayerStatus.isFlameBarrier)
		return;
	//パーフェクトマンフラグが true の場合は終了
	if (gPlayerStatus.isPerfectMan)
		return;

	//プレイヤーとの判定

	//爆弾の座標がプレイヤーの座標と一致した場合
	if (BombPos.x == p->SystemPos.x && BombPos.y == p->SystemPos.y) {
		p->SetPlayerDead(CPlayer::PlayerStateId::DEADplayer);
		IsEnd = true;
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		//爆弾の座標 + 方向
		MapPoint ex_pos{ BombPos.x + std::get<1>(ExplosionPointData[i]).x, BombPos.y + std::get<1>(ExplosionPointData[i]).y };

		//爆弾の威力分ループ
		for (int j = 0; j < std::get<2>(ExplosionPointData[i]); j++) {

			//爆発の座標とプレイヤーの座標が一致した場合
			if (ex_pos.x == p->SystemPos.x && ex_pos.y == p->SystemPos.y) {

				p->SetPlayerDead(CPlayer::PlayerStateId::DEADplayer);
				IsEnd = true;
				break;
			}
			//座標更新
			ex_pos.x += std::get<1>(ExplosionPointData[i]).x;
			ex_pos.y += std::get<1>(ExplosionPointData[i]).y;
		}
	}
}

//爆発アニメーション処理
bool CExplosion::ExplosionAnim(int animMax, int* index)
{
	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < EXPLOSION_ANIM_FRAME)
	{
		AnimCnt++;//インクリメント
		return true;
	}
	//初期化
	AnimCnt = 0;

	//アニメーションの最大値以上の場合は初期化
	if (*index >= animMax - 1)
		return false;

	*index += 1;

	return true;
}

//爆弾描画(レベル別)
//ずれ分の座標x、y、描画個数、縦or横
std::tuple<Point,Point, int> CExplosion::DrawExplosion(float addPosX, float addPosY, int num, ExplosionEffectId dir)
{
	Point movePos{ addPosX * CHIP_SIZE, addPosY * CHIP_SIZE };

	Point drawPos{ pos.x + movePos.x, pos.y + movePos.y };

	MapPoint systemPos{ BombPos.x, BombPos.y };

	ExplosionDir expDir{ ExplosionDir::UP_exp };

	int cnt = 0;
	std::tuple<Point, Point, int> data = std::make_tuple(Point{ 0,0 }, Point{ addPosX, addPosY }, 0);


	//爆発開始位置からのずれを設定
	if (dir == ExplosionEffectId::VERTICAL){
		if (addPosY == -1) {	//上
			std::get<0>(data).x = CHIP_SIZE / 2;
			std::get<0>(data).y = 0;
			expDir = ExplosionDir::UP_exp;
		}
		else{					//下
			std::get<0>(data).x = CHIP_SIZE / 2;
			std::get<0>(data).y = CHIP_SIZE;
			expDir = ExplosionDir::DOWN_exp;
		}
	}
	else {
		if (addPosX == -1) {	//左
			std::get<0>(data).x = 0;
			std::get<0>(data).y = CHIP_SIZE / 2;
			expDir = ExplosionDir::LEFT_exp;
		}
		else {					//右
			std::get<0>(data).x = CHIP_SIZE;
			std::get<0>(data).y = CHIP_SIZE / 2;
			expDir = ExplosionDir::RIGHT_exp;
		}
	}
	
	systemPos = { systemPos.x + static_cast<int>(addPosX),
				  systemPos.y + static_cast<int>(addPosY)
	};

	for (int i = 0; i < num; i++)
	{
		//先の升目がブロックの場合は描画終了
		if (gNowMap[systemPos.y][systemPos.x] == 0)
		{
			std::get<2>(data) = cnt;
			return data;
		}
		if (gNowMap[systemPos.y][systemPos.x] == 1 ||
			(CrashBlockPos[expDir].x == systemPos.x && CrashBlockPos[expDir].y == systemPos.y))
		{
			std::get<2>(data) = cnt + 1;
			CrashBlockPos[expDir].x = systemPos.x; //破壊可能ブロックの座標保存
			CrashBlockPos[expDir].y = systemPos.y; //破壊可能ブロックの座標保存
			return data;
		}	

		//爆発描画
		DrawExtendGraph(drawPos.x - Distance, drawPos.y, drawPos.x + ImgWidth - Distance, drawPos.y + ImgHeight,
			ExplosionImgHandle[EXPLOSION_ANIM_ORDER[AnimIndex] + static_cast<int>(dir)], true);

		//描画座標更新
		drawPos.x += movePos.x;
		drawPos.y += movePos.y;
		//システム上の座標更新
		systemPos.x += static_cast<int>(addPosX);
		systemPos.y += static_cast<int>(addPosY);
		cnt++;
	}

	std::get<2>(data) = cnt;
	return data;
}