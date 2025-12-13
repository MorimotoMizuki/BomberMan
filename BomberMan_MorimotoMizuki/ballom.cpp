#include"obj.h"
#include"function.h"

CBallom::CBallom(Point p, MapPoint system_p)
{
	CBaseEnemy::Constructor(p, system_p); //ベースのコンストラクタ

	//画像を分割
	for (int i = 0; i < BALLOM_IMG_NUM; i++) {
		BallomImgHandle[i] = DerivationGraph(IMGSIZE32 * i, 0, IMGSIZE32, IMGSIZE32, img);
	}

	//死亡時画像の最初の一枚の設定
	EnemyDeadImgHandle[0] = DerivationGraph(IMGSIZE32 * 3, 0, IMGSIZE32, IMGSIZE32, img);

	SPEED = 2.0f; //移動速度
}

int CBallom::Action(vector<unique_ptr<BaseVector>>& base)
{
	//死亡時処理
	if (IsDead) {
		CBaseEnemy::EnemyDead(BALLOM_ANIM_FRAME, 60);
		return 0;
	}

	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return 0;

	Distance = p->Distance; //プレイヤーとの差分を取得

	//爆弾と接触時の座標調整
	CBaseEnemy::HitBomb_PosAdjustment(base);

	//アニメーション処理
	CBaseEnemy::Anim(BALLOM_ANIM_FRAME, BALLOM_ANIM_NUM, &AnimIndex, true);

	//システム上の座標更新 : 中心座標から
	SystemPos = { static_cast<int>((pos.x + ImgWidth / 2) / CHIP_SIZE) ,
				  static_cast<int>(((pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};

	//ランダム移動処理
	RandomMove(base);

	//座標更新
	pos = Add_Point_Vector(pos, vec);

	return 0;
}

void CBallom::Draw()
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
			DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, BallomImgHandle[BALLOM_ANIM_ORDER[AnimIndex]], true);
		//右向き
		else
			DrawExtendGraph(pos.x + ImgWidth - Distance, pos.y, pos.x - Distance, pos.y + ImgHeight, BallomImgHandle[BALLOM_ANIM_ORDER[AnimIndex]], true);
	}
}

CBallom::~CBallom()
{
	CBaseEnemy::Destructor(); //ベースのデストラクタ

	for (int i = 0; i < BALLOM_IMG_NUM; i++)
		DeleteGraph(BallomImgHandle[i]);
}

//敵の死亡時のパラメータ設定
void CBallom::SetEnemyDeadParameter()
{
	if (IsDead) return;

	IsDead = true;
	AnimIndex = 0;
	AnimCnt = 0;
	vec.x = 0.0f;
	vec.y = 0.0f;
}