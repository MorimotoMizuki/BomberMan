#pragma once

#include"objBase.h"

class CBlock : public BaseVector
{
public:

public:

	//���W�AID
	CBlock(Point, int);
	~CBlock();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

private:

};