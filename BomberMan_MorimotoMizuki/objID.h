#pragma once

//�I�u�W�F�N�gID
enum Obj_Id
{
	PLAYER = 1,
	ENEMY,
	BOMB,
	NORMALBLOCK,
	CRASHBLOCK,
	EXPLOSION,
	ITEM,
};

//�I�u�W�F�N�g�\������
enum Pri_Id
{
	pBLOCK,
	pENEMY,
	pBOMB,
	pEXPLOSION,
	pPLAYER,
};

//�����G�t�F�N�gID
enum ExplosionEffectId
{
	eCENTER,	//���S
	eMIDDLE,	//����
	eLEFT,
	eRIGHT,
	eUP,
	eDOWN,
};

// �s��ID
enum Move_Id
{
	UP,		//��
	RIGHT,	//�E
	DOWN,	//��
	LEFT,	//��
	SPACE,	//�X�y�[�X�L�[
	LEFT_CLICK,	//�}�E�X���N���b�N
	ALL_KEY,//�S�ẴL�[����
};
