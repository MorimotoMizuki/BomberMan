#include"obj.h"
#include"function.h"

CPontan::CPontan(Point p, MapPoint system_p)
{
	CBaseEnemy::Constructor(p, system_p); //ベースのコンストラクタ

	//画像を分割
	for (int i = 0; i < PONTAN_IMG_NUM; i++) {
		ImgHandle[i] = DerivationGraph(IMGSIZE32 * i, IMGSIZE32 * 8, IMGSIZE32, IMGSIZE32, img);
	}

	//死亡時画像の最初の一枚の設定
	EnemyDeadImgHandle[0] = DerivationGraph(IMGSIZE32 * 4, IMGSIZE32 * 8, IMGSIZE32, IMGSIZE32, img);

	SPEED = 4.0f; //移動速度
	SCORE = 8000;  //スコア
	STOP_FRAME = 5; //停止フレーム

	//移動制限判定Obj_Id
	HitMoveObj_Id.push_back(Obj_Id::BLOCK);
	HitMoveObj_Id.push_back(Obj_Id::BOMB);

	TrackingParameter.first = false;//追跡距離使用フラグ
}

int CPontan::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return 0;

	Distance = p->Distance; //プレイヤーとの差分を取得

	//死亡時処理
	if (IsDead) {
		CBaseEnemy::EnemyDead(PONTAN_ANIM_FRAME, 60);
		return 0;
	}

	//アニメーション処理
	CBaseEnemy::Anim(PONTAN_ANIM_FRAME, PONTAN_ANIM_NUM, &AnimIndex, true);

	//システム上の座標更新 : 中心座標から
	SystemPos = { static_cast<int>((pos.x + ImgWidth / 2) / CHIP_SIZE) ,
				  static_cast<int>(((pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};
	//システム上の座標更新 : 左上座標から
	MapPoint systemPosL = { static_cast<int>((pos.x) / CHIP_SIZE) ,
							static_cast<int>(((pos.y) - WINDOW_HEADER) / CHIP_SIZE)
	};
	//システム上の座標更新 : 右下座標から
	MapPoint systemPosR = { static_cast<int>((pos.x + ImgWidth - 1 - (SPEED / 2)) / CHIP_SIZE) ,
							static_cast<int>(((pos.y + ImgHeight - 1 - (SPEED / 2)) - WINDOW_HEADER) / CHIP_SIZE)
	};

	//マスぴったりにいる場合
	if (systemPosL.x == systemPosR.x && systemPosL.y == systemPosR.y)
	{
		if (IsPlayerSamePosXY) {
			if (ResetSamePosXYCnt > 5)
			{
				IsPlayerSamePosXY = false;
				ResetSamePosXYCnt = 0;
			}
			else
				ResetSamePosXYCnt++;
		}
		else {
			//プレイヤーのx座標かy座標がこの敵のxy座標と同じになった場合
			if ((p->SystemPos.x == SystemPos.x || p->SystemPos.y == SystemPos.y) && isTrackingPlayer) {
				IsPlayerSamePosXY = true;
			}
		}
	}

	if (IsPlayerSamePosXY) {
		TrackingPlayerPercentage = 10;
	}
	else {
		TrackingPlayerPercentage = 2;
	}

	//プレイヤー追跡処理
	TrackingPlayerMove(p, CHIP_SIZE / SPEED, &isTrackingPlayer, TrackingParameter, TrackingPlayerPercentage);

	if (!isTrackingPlayer) {
		//ランダム移動処理
		RandomMove(base, IsPermitDir, 2);
	}

	//爆弾と接触時の座標調整
	CBaseEnemy::HitBomb_PosAdjustment(base);

	//座標更新
	pos = Add_Point_Vector(pos, vec);

	return 0;
}

void CPontan::Draw()
{
	if (draw_flag)
	{
		//画像描画
		if (IsDead){
			DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, EnemyDeadImgHandle[AnimIndex], true);
		}
		else{
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

CPontan::~CPontan()
{
	CBaseEnemy::Destructor(); //ベースのデストラクタ

	for (int i = 0; i < PONTAN_IMG_NUM; i++)
		DeleteGraph(ImgHandle[i]);
}

//敵の死亡時のパラメータ設定
void CPontan::SetEnemyDeadParameter()
{
	if (IsDead) return;

	IsDead = true;
	AnimIndex = 0;
	AnimCnt = 0;
	vec.x = 0.0f;
	vec.y = 0.0f;
}