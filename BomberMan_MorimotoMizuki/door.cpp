#include"obj.h"
#include"function.h"

CDoor::CDoor(Point p, MapPoint system_p)
{
	img = LoadGraph("image\\door.png");
	SE_StageClear = LoadSoundMem("sound\\StageClear.wav");

	pos = p;

	SystemPos = system_p;

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	ID  = Obj_Id::GOAL;
	pri = Pri_Id::pGOAL;
}

int CDoor::Action(vector<unique_ptr<BaseVector>>& base)
{
	if (gGamePhase != GamePhaseId::PLAING)
		return 0;

	if (gGamePhase == GamePhaseId::GAMECLEAR) return 0;

	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p != nullptr)
	{
		Distance = p->Distance;

		if (IsOpen){
			//プレイヤーの座標がゴールの座標になった場合(許容範囲 : GOAL_IN_DISTANCE)
			if ((pos.x >= p->m_pos.x && pos.x < p->m_pos.x + GOAL_IN_DISTANCE) &&
				(pos.y >= p->m_pos.y && pos.y < p->m_pos.y + GOAL_IN_DISTANCE))
			{
				//SE再生
				My_PlaySoundMem(SE_StageClear, DX_PLAYTYPE_BACK, TRUE, 128);

				//ゲームクリアに設定
				gGamePhase = GamePhaseId::GAMECLEAR;
			}
		}
	}

	return 0;
}

void CDoor::Draw()
{
	//画像描画
	DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, img, true);
}

CDoor::~CDoor()
{

}