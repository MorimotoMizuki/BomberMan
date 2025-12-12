#include"obj.h"
#include"function.h"

COneal::COneal(Point p, MapPoint system_p)
{
	img = LoadGraph("image\\enemy.png");
	//画像を分割
	for (int i = 0; i < ONEAL_IMG_NUM; i++) {
		ImgHandle[i] = DerivationGraph(IMGSIZE32 * i, IMGSIZE32 * 2, IMGSIZE32, IMGSIZE32, img);
	}

	pos = p;

	SystemPos = system_p;

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	ID = Obj_Id::ENEMY;
	pri = gEnemyPri;
	gEnemyPri++;
}

int COneal::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr)
		return 0;
	Distance = p->Distance;

	//死亡処理
	if (IsDead) {

		if (DeadCnt < 60)
			DeadCnt++;
		else
			Anim(ONEAL_ANIM_NUM * 2, &AnimIndex, false);
		return 0;
	}

	//システム上の座標更新 : 左上座標から
	MapPoint systemPosL = { static_cast<int>((pos.x) / CHIP_SIZE) ,
							static_cast<int>(((pos.y) - WINDOW_HEADER) / CHIP_SIZE)
	};
	//システム上の座標更新 : 右下座標から
	MapPoint systemPosR = { static_cast<int>((pos.x + ImgWidth - 1) / CHIP_SIZE) ,
							static_cast<int>(((pos.y + ImgHeight - 1) - WINDOW_HEADER) / CHIP_SIZE)
	};

	if ((systemPosL.x == systemPosR.x && systemPosL.y == systemPosR.y) ||
		move_cnt > 60)
	{
		if ((p->SystemPos.x == systemPosL.x && p->SystemPos.y == systemPosL.y) ||
			(p->SystemPos.x == systemPosR.x && p->SystemPos.y == systemPosR.y) ||
			(p->SystemPos.x == SystemPos.x && p->SystemPos.y == SystemPos.y))
			return 0;

		vec_last_route.clear();
		vec.x = 0.0f;
		vec.y = 0.0f;

		Cell goal{ p->SystemPos.x, p->SystemPos.y };
		Cell start{ SystemPos.x, SystemPos.y };

		auto v_map = ArrToVec(gNowMap);
		//経路の計算
		list<Cell> last_route = ROUTE_CALCULATION2(MAP_CHIP_W, MAP_CHIP_H, start, goal, v_map);

		//vec_last_route.resize(last_route.size());
		for (auto& x : last_route) {
			vec_last_route.push_back(x);
		}

		if (!vec_last_route.empty()) {

			Cell first = vec_last_route[1];

			MapPoint e_v = { first.X - SystemPos.x, first.Y - SystemPos.y };

			vec.x = e_v.x * ONEAL_SPEED;
			vec.y = e_v.y * ONEAL_SPEED;
			move_cnt = 0;
		}
	}
	else
		move_cnt++;

	//アニメーション処理
	Anim(ONEAL_ANIM_NUM, &AnimIndex, true);

	for (int i = 0; i < base.size(); i++)
	{
		//削除対象のオブジェクトはスキップ
		if (!base[i]->FLAG || !base[i]->draw_flag)
			continue;

		//爆弾オブジェクトと判定
		if (base[i]->ID == BOMB)
		{
			if (HitCheck_box(this, base[i].get()))
			{
				MapPoint bPos = (base[i].get())->SystemPos;
				if (bPos.x != SystemPos.x || bPos.y != SystemPos.y)
				{
					pos.x = (SystemPos.x * CHIP_SIZE);
					pos.y = (SystemPos.y * CHIP_SIZE) + WINDOW_HEADER;
				}
			}
		}
	}

	//システム上の座標更新 : 中心座標から
	SystemPos = { static_cast<int>((pos.x + ImgWidth / 2) / CHIP_SIZE) ,
				  static_cast<int>(((pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};

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

	//for (int i = 0; i < vec_last_route.size(); i++)
	//{
	//	int x = vec_last_route[i].X;
	//	int y = vec_last_route[i].Y;
	//	DrawBox(x * CHIP_SIZE - Distance, y * CHIP_SIZE + WINDOW_HEADER, x * CHIP_SIZE + CHIP_SIZE - Distance, y * CHIP_SIZE + CHIP_SIZE + WINDOW_HEADER, 0xff0000, false);
	//}

	//DrawFormatString(pos.x - Distance, pos.y, GetColor(255, 255, 255), "%d\n%d", SystemPos.x, SystemPos.y);
	//DrawFormatString(pos.x - Distance, pos.y, GetColor(255, 255, 255), "%d", AnimIndex);
}

COneal::~COneal()
{
	gKillEnemyNum++; //敵討伐数++

	for (int i = 0; i < ONEAL_IMG_NUM; i++)
		DeleteGraph(ImgHandle[i]);
}

//アニメーション処理
void COneal::Anim(int animMax, int* index, bool loop)
{
	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < ONEAL_ANIM_FRAME)
	{
		AnimCnt++;//インクリメント
		return;
	}
	//初期化
	AnimCnt = 0;

	//アニメーションの最大値以上の場合は初期化
	if (*index >= animMax - 1)
	{
		if (loop)
			*index = 0;
		else
		{
			draw_flag = false;
			FLAG = false;
		}
	}
	else
		*index += 1;
}
