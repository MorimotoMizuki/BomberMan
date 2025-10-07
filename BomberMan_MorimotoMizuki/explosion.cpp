#include"obj.h"
#include"function.h"

CExplosion::CExplosion(Point p, ExplosionEffectId effectId)
{
	img = LoadGraph("image\\normal_block.png");

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;
	pos = p;

	//�G�t�F�N�gID
	EffectId = effectId;

	ID = Obj_Id::EXPLOSION;
	pri = Pri_Id::pEXPLOSION;
}

int CExplosion::Action(vector<unique_ptr<BaseVector>>& base)
{

	return 0;
}

void CExplosion::Draw()
{
	//�摜�`��
	DrawGraph(pos.x, pos.y, img, true);
}

CExplosion::~CExplosion()
{

}