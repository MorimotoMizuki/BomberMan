#include"obj.h"
#include"function.h"
#include"map.h"

CPlayer::CPlayer()
{
	LoadDivGraph("image\\player.png", PLAYER_IMG_NUM, 3, 4, IMGSIZE16, IMGSIZE16, PlayerImgHandle);

	ImgWidth = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	//マップ上の初期位置
	m_pos.x = 64;
	m_pos.y = 160;

	//描画位置
	pos = m_pos;

	ID  = Obj_Id::PLAYER;
	pri = Pri_Id::pPLAYER;
}

int CPlayer::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーの移動処理
	PlayerMove();

	//ブロックオブジェクトと判定
	for (int i = 0; i < base.size(); i++)
	{
		if (base[i]->ID == BLOCK)
		{
			if (((CBlock*)base[i].get())->tipNo >= 0)
			{
				HitCheck_Box_Circle(this, base[i].get(), 32);
			}
		}
	}

	//座標更新
	m_pos = Add_Point_Vector(m_pos, vec);

	//爆弾配置処理
	PutExplosion(base);

	return 0;
}

void CPlayer::Draw()
{
	//画像描画
	DrawExtendGraph(pos.x, pos.y, pos.x + IMGSIZE64, pos.y + IMGSIZE64, PlayerImgHandle[AnimIndex], true);
	
	//デバッグ
	DrawFormatString(WINDOW_WIDTH/2 + 200, 50, GetColor(255, 255, 255), "%f\n%f", m_pos.x, m_pos.y - WINDOW_HEADER);
	DrawFormatString(WINDOW_WIDTH/2 - 100, 50, GetColor(255, 255, 255), "%f\n%f", pos.x, pos.y - WINDOW_HEADER);

	DrawFormatString(WINDOW_WIDTH / 2, 50, GetColor(255, 255, 255), "%d\n%d", static_cast<int>((m_pos.x + ImgWidth / 2) / CHIP_SIZE), static_cast<int>(((m_pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE));
}

CPlayer::~CPlayer()
{
	for (int i = 0; i < PLAYER_IMG_NUM; i++)
		DeleteGraph(PlayerImgHandle[i]);
}

//プレイヤーの移動処理
void CPlayer::PlayerMove()
{
	//移動ベクトル初期化
	vec.x = 0.0f;
	vec.y = 0.0f;

	if (Key_Check(Move_Id::LEFT))
	{
		vec.x = -gPlayerStatus.speed;
		PlayerAnim(AnimMaxId::LEFT, &AnimIndex);
	}
	else if (Key_Check(Move_Id::RIGHT))
	{
		vec.x = gPlayerStatus.speed;
		PlayerAnim(AnimMaxId::RIGHT, &AnimIndex);
	}
	else if (Key_Check(Move_Id::UP))
	{
		vec.y = -gPlayerStatus.speed;
		PlayerAnim(AnimMaxId::UP, &AnimIndex);
	}
	else if (Key_Check(Move_Id::DOWN))
	{
		vec.y = gPlayerStatus.speed;
		PlayerAnim(AnimMaxId::DOWN, &AnimIndex);
	}
}

//プレイヤーのアニメーション処理
void CPlayer::PlayerAnim(AnimMaxId animMaxId, int* index)
{
	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < PLAYER_ANIM_FRAME && KeepAnimMaxId == animMaxId)
	{
		AnimCnt++;//インクリメント
		return;
	}
	//初期化
	AnimCnt = 0;
	//アニメーションID保存
	KeepAnimMaxId = animMaxId;

	//アニメーションの最大値以上の場合は対応する初期値に設定
	if (*index >= animMaxId || *index < animMaxId - PLAYER_ONE_ANIM_NUM)
		*index = animMaxId - PLAYER_ONE_ANIM_NUM;
	else
		*index += 1;
}

//爆弾を置く処理
void CPlayer::PutExplosion(vector<unique_ptr<BaseVector>>& base)
{
	if (!Key_Check(Move_Id::SPACE))
		return;

	//爆弾の個数が設置可能個数以上の場合は終了
	if (gNowBombNum >= gPlayerStatus.bombPutNum)
		return;

	//一つのブロックの情報を取得
	CBlock* b = (CBlock*)Get_obj(base, BLOCK);
	int bMapX = b->pos.x / CHIP_SIZE;
	//マップ上のずれを計算
	float displacement = (bMapX * CHIP_SIZE) - b->pos.x;

	//プレイヤーの中心座標を計算
	Point centerPos = { pos.x + ImgWidth / 2, pos.y + ImgHeight / 2 };

	//爆弾を置くシステム上の座標を計算
	MapPoint putMapPos = { (m_pos.x + ImgWidth / 2) / CHIP_SIZE, ((m_pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE };
	//爆弾生成
	base.emplace_back((unique_ptr<BaseVector>)new CBomb(centerPos, displacement, putMapPos));
}