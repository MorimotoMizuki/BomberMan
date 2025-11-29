#pragma once

#include"objBase.h"

class CBallom : public BaseVector
{
public:

	//‰æ‘œƒnƒ“ƒhƒ‹
	CBallom(Point, int);
	~CBallom();
	int Action(vector<unique_ptr<BaseVector>>&);
	void Draw();

public:


private:

};