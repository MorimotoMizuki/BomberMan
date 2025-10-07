//�Q�[���V�[��
#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"

#include"obj.h"

//�v���C���[�̃X�e�[�^�X
PlayerStatus gPlayerStatus = {
	6.0f,	//speed
	1,		//bombPutNum
	1,		//bombLevel
};
//���݂̐ݒu���Ă��锚�e�̐�
int gNowBombNum = 0;

//�R���X�g���N�^
CGame::CGame(CManager* p) :CScene(p)
{
	base.emplace_back((unique_ptr<BaseVector>)new CPlayer());

	Point bp{ 0,WINDOW_HEADER };
	for (int i = 0; i < 16; i++)
	{
		base.emplace_back((unique_ptr<BaseVector>)new CBlock(bp,Obj_Id::NORMALBLOCK));
		bp.x += 64;
	}

}

//�X�V����
int CGame::Update()
{
	//�w�b�_�[�̔w�i
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEADER, GetColor(173, 173, 173), 1);
	//�Q�[���w�i
	DrawBox(0, WINDOW_HEADER, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(56, 135, 0), 1);

	//�X�V����
	for (int i = 0; i < base.size(); i++)
		base[i]->Action(base);

	//�폜����
	for (auto i = base.begin(); i != base.end();)
		(*i)->FLAG ? i++ : i = base.erase(i);

	//�I�u�W�F�N�g�̃\�[�g����(�N�C�b�N�\�[�g)
	ObjSort_Quick(base, 0, base.size() - 1);

	//list�I�u�W�F�N�g�̍X�V����	
	//for (auto i = base.begin(); i != base.end(); i++)
	//	(*i)->Action(base);

	return 0;
}

//�`�揈��
void CGame::Draw()
{
	//�I�u�W�F�N�g��
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Object_Count = %d", base.size());
	
	for (int i = 0; i < base.size(); i++)
		if(base[i]->FLAG) base[i]->Draw();

	//list�I�u�W�F�N�g�̕`��
	//for (auto i = base.begin(); i != base.end(); i++)
	//	if ((*i)->FLAG) (*i)->Draw();

	//3D���̕`��
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 0, 100),
	//	0x0000ff
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(100, 0, 0),
	//	0xff0000
	//);
	//DrawLine3D(
	//	VGet(0, 0, 0),
	//	VGet(0, 100, 0),
	//	0x00ff00
	//);
}

CGame::~CGame()
{

}
