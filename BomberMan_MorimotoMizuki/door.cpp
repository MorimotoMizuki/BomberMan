#include"obj.h"
#include"function.h"

CDoor::CDoor(Point p, MapPoint system_p)
{
	img = LoadGraph("image\\door.png");

	pos = p;

	SystemPos = system_p;

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	ID  = Obj_Id::GOAL;
	pri = Pri_Id::pGOAL;
}

int CDoor::Action(vector<unique_ptr<BaseVector>>& base)
{
	if (gGamePhase != GamePhaseId::PLAING)
		return 0;

	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p != nullptr)
	{
		Distance = p->Distance;

		//プレイヤーの座標がゴールの座標になった場合(許容範囲 : GOAL_IN_DISTANCE)
		if ((pos.x >= p->m_pos.x && pos.x < p->m_pos.x + GOAL_IN_DISTANCE) &&
			(pos.y >= p->m_pos.y && pos.y < p->m_pos.y + GOAL_IN_DISTANCE))
		{
			//プレイヤーの座標をゴールの座標にする
			p->pos.x = pos.x;
			p->pos.y = pos.y;
			//ゲームクリアに設定
			gGamePhase = GamePhaseId::GAMECLEAR;
		}
	}

	return 0;
}

void CDoor::Draw()
{
	//画像描画
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, img, true);

	//DrawFormatString(WINDOW_WIDTH / 2 - 200, 50, GetColor(255, 255, 255), "%f\n%f", pos.x, pos.y - WINDOW_HEADER);
	DrawFormatString(WINDOW_WIDTH / 2 - 200,50, GetColor(255, 255, 255), "%d\n%d", SystemPos.x, SystemPos.y);
}

CDoor::~CDoor()
{

}