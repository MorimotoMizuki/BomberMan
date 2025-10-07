#pragma once

#include"objBase.h"

class CExplosion : public BaseVector
{
public:

public:

	CExplosion(Point, ExplosionEffectId);
	~CExplosion();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

private:

	//�G�t�F�N�gID
	ExplosionEffectId EffectId;
};