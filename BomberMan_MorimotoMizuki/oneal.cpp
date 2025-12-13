#include"obj.h"
#include"function.h"

COneal::COneal(Point p, MapPoint system_p)
{
	CBaseEnemy::Constructor(p, system_p); //ベースのコンストラクタ

	//画像を分割
	for (int i = 0; i < ONEAL_IMG_NUM; i++) {
		ImgHandle[i] = DerivationGraph(IMGSIZE32 * i, IMGSIZE32 * 2, IMGSIZE32, IMGSIZE32, img);
	}

	//死亡時画像の最初の一枚の設定
	EnemyDeadImgHandle[0] = DerivationGraph(IMGSIZE32 * 4, IMGSIZE32 * 2, IMGSIZE32, IMGSIZE32, img);

	SPEED = 2.0f; //移動速度
}

int COneal::Action(vector<unique_ptr<BaseVector>>& base)
{
	//死亡時処理
	if (IsDead) {
		CBaseEnemy::EnemyDead(ONEAL_ANIM_FRAME, 60);
		return 0;
	}

	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return 0;

	Distance = p->Distance; //プレイヤーとの差分を取得

	//アニメーション処理
	CBaseEnemy::Anim(ONEAL_ANIM_FRAME, ONEAL_ANIM_NUM, &AnimIndex, true);

	//システム上の座標更新 : 中心座標から
	SystemPos = { static_cast<int>((pos.x + ImgWidth / 2) / CHIP_SIZE) ,
				  static_cast<int>(((pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};

	//爆弾と接触時の座標調整
	CBaseEnemy::HitBomb_PosAdjustment(base);

	//プレイヤー追跡処理
	TrackingPlayerMove(p, 32, &isTrackingPlayer);

	if (!isTrackingPlayer) {
		//ランダム移動処理
		RandomMove(base);
	}

	//座標更新
	pos = Add_Point_Vector(pos, vec);

	return 0;
}

void COneal::Draw()
{
	if (!draw_flag) return;

	//画像描画
	if (IsDead)
	{
		DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, ImgHandle[AnimIndex], true);
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

	DrawFormatString(pos.x - Distance, pos.y, GetColor(255, 255, 255), "%d", isTrackingPlayer);

	//for (int i = 0; i < vec_last_route.size(); i++)
	//{
	//	int x = vec_last_route[i].X;
	//	int y = vec_last_route[i].Y;
	//	DrawBox(x * CHIP_SIZE - Distance, y * CHIP_SIZE + WINDOW_HEADER, x * CHIP_SIZE + CHIP_SIZE - Distance, y * CHIP_SIZE + CHIP_SIZE + WINDOW_HEADER, 0xff0000, false);
	//}

	//DrawFormatString(pos.x - Distance, pos.y, GetColor(255, 255, 255), "%d\n%d", SystemPos.x, SystemPos.y);
}

COneal::~COneal()
{
	CBaseEnemy::Destructor(); //ベースのデストラクタ

	for (int i = 0; i < ONEAL_IMG_NUM; i++)
		DeleteGraph(ImgHandle[i]);
}

//敵の死亡時のパラメータ設定
void COneal::SetEnemyDeadParameter()
{
	if (IsDead) return;

	IsDead = true;
	AnimIndex = 0;
	AnimCnt = 0;
	vec.x = 0.0f;
	vec.y = 0.0f;
}