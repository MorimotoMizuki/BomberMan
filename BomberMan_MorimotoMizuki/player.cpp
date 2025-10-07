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
	//�摜�`��
	DrawExtendGraph(pos.x, pos.y, pos.x + ImgWidth, pos.y + ImgHeight, PlayerImgHandle[AnimIndex], true);

	//�f�o�b�O
	//DrawFormatString(0, 150, GetColor(255, 255, 255), "%f", pos.y);
}

CPlayer::~CPlayer()
{
	for (int i = 0; i < PLAYER_IMG_NUM; i++)
		DeleteGraph(PlayerImgHandle[i]);
}

//�v���C���[�̈ړ�����
void CPlayer::PlayerMove()
{
	//�ړ��x�N�g��������
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

	//���W�X�V
	pos = Add_Point_Vector(pos, vec);

	//�͈͊O����
	if (pos.x < 0)
		pos.x = 0;
	if (pos.x + ImgWidth > WINDOW_WIDTH)
		pos.x = (float)WINDOW_WIDTH - (float)ImgWidth;
	if (pos.y < 0)
		pos.y = 0;
	if (pos.y + ImgHeight > WINDOW_HEIGHT)
		pos.y = (float)WINDOW_HEIGHT - (float)ImgHeight;
}

//�v���C���[�̃A�j���[�V��������
void CPlayer::PlayerAnim(AnimMaxId animMaxId, int* index)
{
	//�A�j���[�V�����J�E���g���萔�����̏ꍇ�͏I��
	if (AnimCnt < PLAYER_ANIM_FRAME && KeepAnimMaxId == animMaxId)
	{
		AnimCnt++;//�C���N�������g
		return;
	}
	//������
	AnimCnt = 0;
	//�A�j���[�V����ID�ۑ�
	KeepAnimMaxId = animMaxId;

	//�A�j���[�V�����̍ő�l�ȏ�̏ꍇ�͑Ή����鏉���l�ɐݒ�
	if (*index >= animMaxId || *index < animMaxId - PLAYER_ONE_ANIM_NUM)
		*index = animMaxId - PLAYER_ONE_ANIM_NUM;
	else
		*index += 1;
}

//���e��u������
void CPlayer::PutExplosion(vector<unique_ptr<BaseVector>>& base)
{
	if (!Key_Check(Move_Id::SPACE))
		return;

	//���e�̌����ݒu�\���ȏ�̏ꍇ�͏I��
	if (gNowBombNum >= gPlayerStatus.bombPutNum)
		return;

	//���e����
	base.emplace_back((unique_ptr<BaseVector>)new CBomb(pos));
}