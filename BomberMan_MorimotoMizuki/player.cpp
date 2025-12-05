#include"obj.h"
#include"function.h"
#include"map.h"

CPlayer::CPlayer()
{
	LoadDivGraph("image\\player.png", PLAYER_IMG_NUM, 3, 6, IMGSIZE16, IMGSIZE16, PlayerImgHandle);

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	//マップ上の初期位置
	m_pos.x = 64;
	m_pos.y = 160;

	//描画位置
	pos = m_pos;

	//システム上の座標
	SystemPos = { static_cast<int>((m_pos.x + ImgWidth / 2) / CHIP_SIZE) ,
				  static_cast<int>(((m_pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};

	ID  = Obj_Id::PLAYER;
	pri = Pri_Id::pPLAYER;
}

int CPlayer::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーの死亡処理
	PlayerDead();

	if (gGamePhase != GamePhaseId::PLAING)
		return 0;

	//プレイヤーの移動処理
	PlayerMove();

	//プレイヤーの当たり判定
	PlayerHit(base);

	//座標更新
	m_pos = Add_Point_Vector(m_pos, vec);

	//カメラをプレイヤー座標
	Point camera_pos = m_pos;

	//カメラの領域判定
	//左端
	if (camera_pos.x < DRAW_CHIP_W * CHIP_SIZE / 2)
		camera_pos.x = DRAW_CHIP_W * CHIP_SIZE / 2;
	//右端
	if (camera_pos.x > MAP_CHIP_W * CHIP_SIZE - DRAW_CHIP_W * CHIP_SIZE / 2 - CHIP_SIZE)
		camera_pos.x = MAP_CHIP_W * CHIP_SIZE - DRAW_CHIP_W * CHIP_SIZE / 2 - CHIP_SIZE;

	//上端
	if (camera_pos.y < DRAW_CHIP_H * CHIP_SIZE / 2)
		camera_pos.y = DRAW_CHIP_H * CHIP_SIZE / 2;
	//下端
	if (camera_pos.y > MAP_CHIP_H * CHIP_SIZE - DRAW_CHIP_H * CHIP_SIZE / 2)
		camera_pos.y = MAP_CHIP_H * CHIP_SIZE - DRAW_CHIP_H * CHIP_SIZE / 2;

	pos.x = m_pos.x - camera_pos.x + DRAW_CHIP_W * CHIP_SIZE / 2;
	pos.y = m_pos.y - camera_pos.y + DRAW_CHIP_H * CHIP_SIZE / 2;

	//差分を計算
	Distance = m_pos.x - pos.x;

	//システム上の座標更新 : 中心座標から
	SystemPos = { static_cast<int>((m_pos.x + ImgWidth / 2) / CHIP_SIZE) ,
			  static_cast<int>(((m_pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};

	//爆弾配置処理
	PutExplosion(base);

	KeyCheck[0] = Key_Check(Move_Id::SPACE);

	return 0;
}

void CPlayer::Draw()
{
	if (!draw_flag) return;

	//画像描画
	DrawExtendGraph(pos.x, pos.y, pos.x + ImgWidth, pos.y + ImgHeight, PlayerImgHandle[AnimIndex], true);
	
	//DrawFormatString(WINDOW_WIDTH / 2 + 300, 50, GetColor(255, 255, 255), "x:%f y:%f\nx:%f y:%f", m_pos.x, m_pos.y - WINDOW_HEADER, m_pos.x + ImgWidth - 1, m_pos.y + ImgHeight - WINDOW_HEADER -1 );

	//デバッグ
	//DrawFormatString(WINDOW_WIDTH/2 + 300, 50, GetColor(255, 255, 255), "%f\n%f", m_pos.x, m_pos.y - WINDOW_HEADER);
	//DrawFormatString(WINDOW_WIDTH/2 - 100, 50, GetColor(255, 255, 255), "%f\n%f", pos.x, pos.y - WINDOW_HEADER);

	DrawFormatString(WINDOW_WIDTH / 2, 50, GetColor(255, 255, 255), "%d\n%d", SystemPos.x, SystemPos.y);

	//DrawBox(m_pos.x, m_pos.y, m_pos.x + ImgWidth, m_pos.y + ImgHeight, GetColor(255, 0, 0), false);
}

CPlayer::~CPlayer()
{
	for (int i = 0; i < PLAYER_IMG_NUM; i++)
		DeleteGraph(PlayerImgHandle[i]);
}

//安全な座標かチェックする
bool CPlayer::IsValidMapPos(MapPoint p)
{
	if (p.x >= 0 && p.x < MAP_CHIP_W &&
		p.y >= 0 && p.y < MAP_CHIP_H)
		return true;
	else
		return false;
}

//プレイヤーの当たり判定
void CPlayer::PlayerHit(vector<unique_ptr<BaseVector>>& base)
{
	//オブジェクトと判定
	for (int i = 0; i < base.size(); i++)
	{
		//削除対象のオブジェクトはスキップ
		if (!base[i]->FLAG || !base[i]->draw_flag)
			continue;

		//ブロックとの判定 : 行動制限のみ
		if (base[i]->ID == BLOCK)
		{
			if (((CBlock*)base[i].get())->tipNo >= 0)
			{
				HitCheck_Box_Circle(this, base[i].get(), 32, Distance);
			}
		}
		//爆弾との判定 : 行動制限のみ
		else if (base[i]->ID == BOMB)
		{
			//システム上の座標 : 左上の座標から
			MapPoint systemPosL = { static_cast<int>(m_pos.x / CHIP_SIZE) ,
								   static_cast<int>((m_pos.y - WINDOW_HEADER) / CHIP_SIZE)
			};
			//システム上の座標 : 右下の座標から
			MapPoint systemPosR = { static_cast<int>((m_pos.x + ImgWidth  - 1) / CHIP_SIZE) ,
								   static_cast<int>(((m_pos.y + ImgHeight - 1) - WINDOW_HEADER) / CHIP_SIZE)
			};
			if (IsValidMapPos(systemPosL) && IsValidMapPos(systemPosR))
			{
				//左上の座標と右下の座標と中心座標のどちらも爆弾がない場合
				if ((gNowMap[systemPosL.y][systemPosL.x] != BOMB) &&
					(gNowMap[systemPosR.y][systemPosR.x] != BOMB) && 
					(gNowMap[SystemPos.y][SystemPos.x] != BOMB))
				{
					HitCheck_Box_Circle(this, base[i].get(), 32, Distance);
				}
			}
		}
		//敵との判定
		else if (base[i]->ID == ENEMY)
		{
			if (PlayerState == PlayerStateId::PLAYplayer) {

				if (SystemPos.x == ((CBallom*)base[i].get())->SystemPos.x &&
					SystemPos.y == ((CBallom*)base[i].get())->SystemPos.y)
				{
					//	SetPlayerDead(PlayerStateId::DEADplayer); //プレイヤー死亡
				}
			}
		}
	}
}

//プレイヤーの移動処理
void CPlayer::PlayerMove()
{
	if (PlayerState != PlayerStateId::PLAYplayer)
		return;

	//移動ベクトル初期化
	vec.x = 0.0f;
	vec.y = 0.0f;

	if (Key_Check(Move_Id::LEFT))
	{
		vec.x = -gPlayerStatus.speed;
		PlayerAnim(AnimMaxId::LEFT, PLAYER_ANIM_FRAME, &AnimIndex);
	}
	else if (Key_Check(Move_Id::RIGHT))
	{
		vec.x = gPlayerStatus.speed;
		PlayerAnim(AnimMaxId::RIGHT, PLAYER_ANIM_FRAME, &AnimIndex);
	}
	else if (Key_Check(Move_Id::UP))
	{
		vec.y = -gPlayerStatus.speed;
		PlayerAnim(AnimMaxId::UP, PLAYER_ANIM_FRAME, &AnimIndex);
	}
	else if (Key_Check(Move_Id::DOWN))
	{
		vec.y = gPlayerStatus.speed;
		PlayerAnim(AnimMaxId::DOWN, PLAYER_ANIM_FRAME, &AnimIndex);
	}
}

//プレイヤーのアニメーション処理
bool CPlayer::PlayerAnim(AnimMaxId animMaxId,int animFrame, int* index, bool isLoop)
{
	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < animFrame && KeepAnimMaxId == animMaxId)
	{
		AnimCnt++;//インクリメント
		return false;
	}

	//初期化
	AnimCnt = 0;
	//アニメーションID保存
	KeepAnimMaxId = animMaxId;

	if (!isLoop)
	{
		if (*index >= animMaxId)
			return true;
		else
			*index += 1;
	}
	else
	{
		//アニメーションの最大値以上の場合は対応する初期値に設定
		if (*index >= animMaxId || *index < animMaxId - PLAYER_ONE_ANIM_NUM)
			*index = animMaxId - PLAYER_ONE_ANIM_NUM;
		else
			*index += 1;
	}

	return false;
}

//爆弾を置く処理
void CPlayer::PutExplosion(vector<unique_ptr<BaseVector>>& base)
{
	//スペースキー入力チェック
	if (!Key_Check(Move_Id::SPACE))
		return;

	//連続入力回避
	if (KeyCheck[0] == true)
		return;

	//爆弾の個数が設置可能個数以上の場合は終了
	if (gNowBombNum >= gPlayerStatus.bombPutNum)
		return;

	//一つのブロックの情報を取得
	CBlock* b = (CBlock*)Get_obj(base, BLOCK);
	int bMapX = b->pos.x / CHIP_SIZE;

	//爆弾を置くシステム上の座標を計算
	MapPoint putMapPos = { (m_pos.x + ImgWidth / 2) / CHIP_SIZE, ((m_pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE };

	IsPutBomb = true;

	//現在のマップに爆弾を配置
	gNowMap[SystemPos.y][SystemPos.x] = Obj_Id::BOMB;

	//爆弾生成
	base.emplace_back((unique_ptr<BaseVector>)new CBomb(SystemPos));
}

//プレイヤーの死亡時処理
void CPlayer::PlayerDead()
{
	if (PlayerState != PlayerStateId::DEADplayer)
		return;

	//ベクトルを初期化
 	vec.x = 0.0f;
	vec.y = 0.0f;

	if (PlayerAnim(AnimMaxId::DEAD, PLAYER_ANIM_FRAME, &AnimIndex, false))
	{
		//ゲームオーバー状態
		gGamePhase = GamePhaseId::GAMEOVER;
		//描画しない
		draw_flag = false;
		PlayerState = PlayerStateId::NONEplayer;
	}
}