#pragma once

#include"objBase.h"

class CBomb : public BaseVector
{
public:

	//���e�̃A�j���[�V�����t���[����
	static constexpr auto BOMB_ANIM_FRAME = 16;

	//���e�摜�̐�
	static constexpr auto BOMB_IMG_NUM = 3;

	//���e�̃A�j���[�V������
	static constexpr auto BOMB_ANIM_NUM = 4;

	//���e�A�j���[�V�����̏���
	static constexpr int BOMB_ANIM_ORDER[BOMB_ANIM_NUM] = { 0,1,2,1 };

public:

	CBomb(Point, int = 180);
	~CBomb();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//���e�A�j���[�V��������
	void BombAnim(int, int*);

	//�����G�t�F�N�g
	void ExplosionEffect(vector<unique_ptr<BaseVector>>&);

private:

	//���e�摜�n���h���z��
	int BombImgHandle[BOMB_IMG_NUM];
	//�A�j���[�V�����̉摜�C���f�b�N�X
	int AnimIndex{ 0 };
	//�A�j���[�V�����J�E���g
	int AnimCnt{ BOMB_ANIM_FRAME };

	//��������(�t���[����)
	int SurvivalFrame{ 0 };
	//���S����(�t���[����)
	int DiedFrame{ 0 };
};