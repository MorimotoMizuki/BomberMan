#include"obj.h"
#include"function.h"

CDahl::CDahl(Point p, MapPoint system_p)
{
	CBaseEnemy::Constructor(p, system_p); //ベースのコンストラクタ

	//画像を分割
	for (int i = 0; i < DAHL_IMG_NUM; i++) {
		ImgHandle[i] = DerivationGraph(IMGSIZE32 * i, IMGSIZE32 * 3, IMGSIZE32, IMGSIZE32, img);
	}

	//死亡時画像の最初の一枚の設定
	EnemyDeadImgHandle[0] = DerivationGraph(IMGSIZE32 * 4, IMGSIZE32 * 3, IMGSIZE32, IMGSIZE32, img);

	SPEED = 4.0f; //移動速度
	SCORE = 400;  //スコア
	STOP_FRAME = 5; //停止フレーム

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

int CDahl::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return 0;

	Distance = p->Distance; //プレイヤーとの差分を取得

	if (IsDead) {
		CBaseEnemy::EnemyDead(DAHL_ANIM_FRAME, 60);
		return 0;
	}

	//爆弾と接触時の座標調整
	CBaseEnemy::HitBomb_PosAdjustment(base);

	//アニメーション処理
	CBaseEnemy::Anim(DAHL_ANIM_FRAME, DAHL_ANIM_NUM, &AnimIndex, true);

	//システム上の座標更新 : 中心座標から
	SystemPos = { static_cast<int>((pos.x + ImgWidth / 2) / CHIP_SIZE) ,
				  static_cast<int>(((pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};

	//ランダム移動処理
	RandomMove(base, IsPermitDir);

	//方向転換
	if (vec.x == 0.0f && vec.y == 0.0f)
	{
		//システム上の座標更新 : 左上座標から
		MapPoint systemPosL = { static_cast<int>((pos.x) / CHIP_SIZE) ,
								static_cast<int>(((pos.y) - WINDOW_HEADER) / CHIP_SIZE)
		};
		//システム上の座標更新 : 右下座標から
		MapPoint systemPosR = { static_cast<int>((pos.x + ImgWidth - 1) / CHIP_SIZE) ,
								static_cast<int>(((pos.y + ImgHeight - 1) - WINDOW_HEADER) / CHIP_SIZE)
		};
		if (systemPosL.x == systemPosR.x && systemPosL.y == systemPosR.y)
		{
			if (DirChangeCnt > STOP_FRAME) {

				if (IsPermitDir[MoveDir::LEFT]) {
					IsPermitDir[MoveDir::LEFT] = false; IsPermitDir[MoveDir::RIGHT] = false;
					IsPermitDir[MoveDir::UP] = true;	IsPermitDir[MoveDir::DOWN] = true;
				}
				else {
					IsPermitDir[MoveDir::LEFT] = true;	IsPermitDir[MoveDir::RIGHT] = true;
					IsPermitDir[MoveDir::UP] = false;	IsPermitDir[MoveDir::DOWN] = false;
				}
				DirChangeCnt = 0;
			}
			else
				DirChangeCnt++;
		}
	}
	else
		DirChangeCnt = 0;

	//座標更新
	pos = Add_Point_Vector(pos, vec);

	return 0;
}

void CDahl::Draw()
{
	if (!draw_flag) return;

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

	DrawFormatString(pos.x - Distance, pos.y, GetColor(255, 255, 255), "%d", IsPermitDir[MoveDir::LEFT]);

	//スコアの表示
	if (IsDrawScore)
		DrawScore();
}

CDahl::~CDahl()
{
	CBaseEnemy::Destructor(); //ベースのデストラクタ

	for (int i = 0; i < DAHL_IMG_NUM; i++)
		DeleteGraph(ImgHandle[i]);
}

//敵の死亡時のパラメータ設定
void CDahl::SetEnemyDeadParameter()
{
	if (IsDead) return;

	IsDead = true;
	AnimIndex = 0;
	AnimCnt = 0;
	vec.x = 0.0f;
	vec.y = 0.0f;
}