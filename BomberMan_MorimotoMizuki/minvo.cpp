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

	SPEED = 2.0f; //移動速度
	SCORE = 800;  //スコア
	STOP_FRAME = 5; //停止フレーム
}

int CMinvo::Action(vector<unique_ptr<BaseVector>>& base)
{

	return 0;
}

void CMinvo::Draw()
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