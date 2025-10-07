#include"obj.h"
#include"function.h"

CBomb::CBomb(Point p, int diedFrame)
{
	LoadDivGraph("image\\bomb.png", BOMB_IMG_NUM, 3, 1, IMGSIZE16, IMGSIZE16, BombImgHandle);

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	pos = p;
	DiedFrame = diedFrame;

	ID = Obj_Id::BOMB;
	pri = Pri_Id::pBOMB;

	//�ݒu���Ă��锚�e�̐� : ����
	gNowBombNum++;
}

int CBomb::Action(vector<unique_ptr<BaseVector>>& base)
{
	SurvivalFrame++;
	if (SurvivalFrame >= DiedFrame)
	{
		ExplosionEffect(base);
		FLAG = false;
	}

	//���e�A�j���[�V����
	BombAnim(BOMB_ANIM_NUM, &AnimIndex);

	return 0;
}

void CBomb::Draw()
{
	//�摜�`��
	DrawExtendGraph(pos.x, pos.y, pos.x + ImgWidth, pos.y + ImgHeight, BombImgHandle[BOMB_ANIM_ORDER[AnimIndex]], true);
}

//���e�A�j���[�V��������
void CBomb::BombAnim(int animMax, int* index)
{
	//�A�j���[�V�����J�E���g���萔�����̏ꍇ�͏I��
	if (AnimCnt < BOMB_ANIM_FRAME)
	{
		AnimCnt++;//�C���N�������g
		return;
	}
	//������
	AnimCnt = 0;

	//�A�j���[�V�����̍ő�l�ȏ�̏ꍇ�͏�����
	if (*index >= animMax - 1)
		*index = 0;
	else
		*index += 1;

}


CBomb::~CBomb()
{
	//���ݐݒu���Ă��锚�e�̐� : ����
	gNowBombNum--;
}

//�����G�t�F�N�g
void CBomb::ExplosionEffect(vector<unique_ptr<BaseVector>>& base)
{
	//�����G�t�F�N�g���� 
	base.emplace_back((unique_ptr<BaseVector>)new CExplosion(pos, ExplosionEffectId::eCENTER));

	//���e���x���ʂŐ�������G�t�F�N�g�̕ύX
	switch (gPlayerStatus.bombLevel)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}

}