#include"obj.h"
#include"function.h"

CMinvo::CMinvo(Point p, MapPoint system_p)
{
	CBaseEnemy::Constructor(p, system_p); //ベースのコンストラクタ

	//画像を分割
	for (int i = 0; i < MINVO_IMG_NUM; i++) {
		ImgHandle[i] = DerivationGraph(IMGSIZE32 * i, IMGSIZE32 * 4, IMGSIZE32, IMGSIZE32, img);
	}

	//死亡時画像の最初の一枚の設定
	EnemyDeadImgHandle[0] = DerivationGraph(IMGSIZE32 * 4, IMGSIZE32 * 4, IMGSIZE32, IMGSIZE32, img);

	SPEED = 4.0f; //移動速度
	SCORE = 800;  //スコア
	STOP_FRAME = 10; //停止フレーム

	//移動制限判定Obj_Id
	HitMoveObj_Id.push_back(Obj_Id::BLOCK);
	HitMoveObj_Id.push_back(Obj_Id::CRASH_BLOCK);
	HitMoveObj_Id.push_back(Obj_Id::BOMB);

	TrackingParameter.first = true;	//追跡距離使用フラグ
	TrackingParameter.second = 5;	//追跡距離

	//移動方向設定
	int vecNum = Range_Random_Number(0, 3);

	if (vecNum == MoveDir::LEFT || vecNum == MoveDir::RIGHT) {
		IsPermitDir[MoveDir::UP] = false;
		IsPermitDir[MoveDir::DOWN] = false;
	}
	else {
		IsPermitDir[MoveDir::LEFT] = false;
		IsPermitDir[MoveDir::RIGHT] = false;
	}
}

int CMinvo::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return 0;

	Distance = p->Distance; //プレイヤーとの差分を取得

	if (IsDead) {
		CBaseEnemy::EnemyDead(MINVO_ANIM_FRAME, 60);
		return 0;
	}

	//爆弾と接触時の座標調整
	CBaseEnemy::HitBomb_PosAdjustment(base);

	//アニメーション処理
	CBaseEnemy::Anim(MINVO_ANIM_FRAME, MINVO_ANIM_NUM, &AnimIndex, true);

	//システム上の座標更新 : 中心座標から
	SystemPos = { static_cast<int>((pos.x + ImgWidth / 2) / CHIP_SIZE) ,
				  static_cast<int>(((pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};

	//プレイヤー追跡処理
	CBaseEnemy::TrackingPlayerMove(p, CHIP_SIZE / SPEED, &isTrackingPlayer, TrackingParameter, 1);

	if (!isTrackingPlayer) {
		//ランダム移動処理
		CBaseEnemy::RandomMove(base, IsPermitDir, 0);

		//方向転換
		if (vec.x == 0.0f && vec.y == 0.0f) {
			DirChangeCnt = CBaseEnemy::SetLineMoveIsDir(DirChangeCnt, 1);
		}
		else
			DirChangeCnt = 0;
	}

	//座標更新
	pos = Add_Point_Vector(pos, vec);
	return 0;
}

void CMinvo::Draw()
{
	if (draw_flag)
	{
		//画像描画
		if (IsDead)
		{
			DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, EnemyDeadImgHandle[AnimIndex], true);
		}
		else
		{
			//左向き
			if (vec.x < 0.0f)
				DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, ImgHandle[AnimIndex], true);
			//右向き
			else
				DrawExtendGraph(pos.x + ImgWidth - Distance, pos.y, pos.x - Distance, pos.y + ImgHeight, ImgHandle[AnimIndex], true);
		}
	}
	//スコアの表示
	if (IsDrawScore)
		DrawScore();
}

CMinvo::~CMinvo()
{
	CBaseEnemy::Destructor(); //ベースのデストラクタ

	for (int i = 0; i < MINVO_IMG_NUM; i++)
		DeleteGraph(ImgHandle[i]);
}

//敵の死亡時のパラメータ設定
void CMinvo::SetEnemyDeadParameter()
{
	if (IsDead) return;

	IsDead = true;
	AnimIndex = 0;
	AnimCnt = 0;
	vec.x = 0.0f;
	vec.y = 0.0f;
}