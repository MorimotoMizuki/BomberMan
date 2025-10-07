#pragma once

#include"objBase.h"

class CPlayer : public BaseVector
{
public:

	//�v���C���[�̃A�j���[�V�����t���[����
	static constexpr auto PLAYER_ANIM_FRAME = 8;

	//�v���C���[�̉摜�̐�
	static constexpr auto PLAYER_IMG_NUM = 12;

	//�v���C���[�̈�̃A�j���[�V�����Ɏg�p���鐔(�z��̓Y�����ɂ���悤��-1)
	static constexpr auto PLAYER_ONE_ANIM_NUM = 2;


	enum AnimMaxId
	{
		LEFT = 2,
		RIGHT = 5,
		UP = 8,
		DOWN = 11,
	};

public:

	CPlayer();
	~CPlayer();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

	//�v���C���[�̈ړ�����
	void PlayerMove();

	//�v���C���[�̃A�j���[�V��������
	void PlayerAnim(AnimMaxId, int*);

	//���e��u������
	void PutExplosion(vector<unique_ptr<BaseVector>>&);

private:

	//�v���C���[�̉摜�n���h���z��
	int PlayerImgHandle[PLAYER_IMG_NUM];
	//�A�j���[�V�����̉摜�C���f�b�N�X
	int AnimIndex{ 0 };
	//�A�j���[�V�����J�E���g
	int AnimCnt{ PLAYER_ANIM_FRAME };
	//���O�̃A�j���[�V����ID�ۑ��p
	AnimMaxId KeepAnimMaxId{ AnimMaxId::LEFT };

};