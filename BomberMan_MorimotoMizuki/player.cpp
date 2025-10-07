#include"obj.h"
#include"function.h"

CPlayer::CPlayer()
{
	LoadDivGraph("image\\player.png", PLAYER_IMG_NUM, 3, 4, IMGSIZE16, IMGSIZE16, PlayerImgHandle);
	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	ID  = Obj_Id::PLAYER;
	pri = Pri_Id::pPLAYER;
}

int CPlayer::Action(vector<unique_ptr<BaseVector>>& base)
{
	PlayerMove();

	PutExplosion(base);

	return 0;
}

void CPlayer::Draw()
{
	//画像描画
	DrawExtendGraph(pos.x, pos.y, pos.x + ImgWidth, pos.y + ImgHeight, PlayerImgHandle[AnimIndex], true);

	//デバッグ
	//DrawFormatString(0, 150, GetColor(255, 255, 255), "%f", pos.y);
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

	//座標更新
	pos = Add_Point_Vector(pos, vec);

	//範囲外処理
	if (pos.x < 0)
		pos.x = 0;
	if (pos.x + ImgWidth > WINDOW_WIDTH)
		pos.x = (float)WINDOW_WIDTH - (float)ImgWidth;
	if (pos.y < 0)
		pos.y = 0;
	if (pos.y + ImgHeight > WINDOW_HEIGHT)
		pos.y = (float)WINDOW_HEIGHT - (float)ImgHeight;
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

	//爆弾生成
	base.emplace_back((unique_ptr<BaseVector>)new CBomb(pos));
}