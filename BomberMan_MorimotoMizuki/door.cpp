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

	gDoorExplosionEnemyNum = 0;
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

		if (IsOpen)
		{
			//ドアの座標にクラッシュブロックではない場合
			if (gNowMap[SystemPos.y][SystemPos.x] != Obj_Id::CRASH_BLOCK)
			{
				//プレイヤーの座標がゴールの座標になった場合(許容範囲 : GOAL_IN_DISTANCE)
				if ((pos.x >= p->m_pos.x && pos.x < p->m_pos.x + GOAL_IN_DISTANCE) &&
					(pos.y >= p->m_pos.y && pos.y < p->m_pos.y + GOAL_IN_DISTANCE))
				{
					//SE再生
					My_PlaySoundMem(SE_StageClear, DX_PLAYTYPE_BACK, TRUE, MusicVolume::SE_StageClear);

					//ゲームクリアに設定
					gGamePhase = GamePhaseId::GAMECLEAR;
				}
			}
		}
	}

	if (IsDoorExplosion) {
		ExplosionCnt++;
		if (ExplosionCnt > 60) {
			DoorExplosion(base);
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

//扉爆破処理
void CDoor::DoorExplosion(vector<unique_ptr<BaseVector>>& base)
{
	Point p{ SystemPos.x * CHIP_SIZE, SystemPos.y * CHIP_SIZE + WINDOW_HEADER };

	for (int i = 0; i < 3; i++) {
		//バロム生成
		base.emplace_back((unique_ptr<BaseVector>) new CBallom(p, SystemPos, true));
		gDoorExplosionEnemyNum++;
	}

	DoorExplosionNum++; //ドア爆破回数カウント

	IsDoorExplosion = false;
	ExplosionCnt = 0;
}