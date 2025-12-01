#include"obj.h"
#include"function.h"

CBallom::CBallom(Point p, MapPoint system_p)
{
	LoadDivGraph("image\\enemy.png", BALLOM_IMG_NUM, 4, 1, IMGSIZE32, IMGSIZE32, BallomImgHandle);

	pos = p;

	SystemPos = system_p;

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	ID = Obj_Id::ENEMY;
	pri = Pri_Id::pENEMY;

	//バロムの移動処理
	BallomMove();
}

int CBallom::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p != nullptr)
		Distance = p->Distance;

	//システム上の座標更新 : 中心座標から
	SystemPos = { static_cast<int>((pos.x + ImgWidth / 2) / CHIP_SIZE) ,
				  static_cast<int>(((pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};


	//バロムのアニメーション処理
	BallomAnim(BALLOM_ANIM_NUM, &AnimIndex);

	pos = Add_Point_Vector(pos, vec);

	return 0;
}

void CBallom::Draw()
{
	//画像描画
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, BallomImgHandle[BALLOM_ANIM_ORDER[AnimIndex]], true);
}

CBallom::~CBallom()
{
	for (int i = 0; i < BALLOM_IMG_NUM; i++)
		DeleteGraph(BallomImgHandle[i]);
}

//バロムの移動処理
void CBallom::BallomMove()
{
	vector<int> moveDir;

	//左右上下
	MapPoint addVec[4] = {
		{-1,0},
		{1,0},
		{0,-1},
		{0,1}
	};

	if (SystemPos.x < MAP_CHIP_W - 1 && SystemPos.x > 0 &&
		SystemPos.y < MAP_CHIP_H - 1 && SystemPos.y > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			MapPoint systemPos = { SystemPos.x + addVec[i].x, SystemPos.y + addVec[i].y };
			if (gNowMap[systemPos.y][systemPos.x] == -1)
				moveDir.push_back(i);
		}
	}

	int randomNum = Range_Random_Number(0, moveDir.size() -1);

	switch (moveDir[randomNum])
	{
		case 1:
			vec.x = addVec[moveDir[randomNum]].x;
			vec.y = addVec[moveDir[randomNum]].y;
			break;
		case 2:
			vec.x = addVec[moveDir[randomNum]].x;
			vec.y = addVec[moveDir[randomNum]].y;
			break;
		case 3:
			vec.x = addVec[moveDir[randomNum]].x;
			vec.y = addVec[moveDir[randomNum]].y;
			break;
		case 4:
			vec.x = addVec[moveDir[randomNum]].x;
			vec.y = addVec[moveDir[randomNum]].y;
			break;
	}
}

//バロムアニメーション処理
void CBallom::BallomAnim(int animMax, int* index)
{
	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < BALLOM_ANIM_FRAME)
	{
		AnimCnt++;//インクリメント
		return;
	}
	//初期化
	AnimCnt = 0;

	//アニメーションの最大値以上の場合は初期化
	if (*index >= animMax - 1)
		*index = 0;
	else
		*index += 1;
}
