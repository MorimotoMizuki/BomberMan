#include"obj.h"
#include"function.h"

CBallom::CBallom(Point p, MapPoint system_p)
{
	LoadDivGraph("image\\enemy.png", BALLOM_IMG_NUM, 4, 2, IMGSIZE32, IMGSIZE32, BallomImgHandle);

	pos = p;

	SystemPos = system_p;

	ImgWidth  = IMGSIZE64;
	ImgHeight = IMGSIZE64;

	ID = Obj_Id::ENEMY;
	pri = Pri_Id::pENEMY;
}

int CBallom::Action(vector<unique_ptr<BaseVector>>& base)
{
	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p != nullptr)
		Distance = p->Distance;

	//死亡処理
	if (IsDead) {
		
		if (DeadCnt < 60)
			DeadCnt++;
		else
			BallomAnim(BALLOM_ANIM_NUM * 2, &AnimIndex, false);
		return 0;
	}

	//システム上の座標更新 : 中心座標から
	SystemPos = { static_cast<int>((pos.x + ImgWidth / 2) / CHIP_SIZE) ,
				  static_cast<int>(((pos.y + ImgHeight / 2) - WINDOW_HEADER) / CHIP_SIZE)
	};

	//バロムの移動処理
	BallomMove(base);

	//バロムのアニメーション処理
	BallomAnim(BALLOM_ANIM_NUM, &AnimIndex, true);

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
		DrawExtendGraph(pos.x - Distance, pos.y, pos.x + ImgWidth - Distance, pos.y + ImgHeight, BallomImgHandle[AnimIndex], true);
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

	DrawFormatString(pos.x - Distance, pos.y, GetColor(255, 255, 255), "%d\n%d", SystemPos.x, SystemPos.y);

	//DrawFormatString(pos.x, pos.y, GetColor(255, 255, 255), "%d", AnimIndex);
}

CBallom::~CBallom()
{
	gKillEnemyNum++; //敵討伐数++

	for (int i = 0; i < BALLOM_IMG_NUM; i++)
		DeleteGraph(BallomImgHandle[i]);
}

//バロムの移動処理
void CBallom::BallomMove(vector<unique_ptr<BaseVector>>& base)
{
	if (IsDead) return;

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
		if (SystemPos.x < MAP_CHIP_W - 1 && SystemPos.x > 0 &&
			SystemPos.y < MAP_CHIP_H - 1 && SystemPos.y > 0)
		{
			Vector normalizeVec = Vector_Normalize(vec);

			//進行方向が空白ではない場合は移動方向変更
			for (int i = 0; i < 4; i++)
			{
				if (normalizeVec.x == ADD_VEC[i].x && normalizeVec.y == ADD_VEC[i].y)
				{
					MapPoint systemPos = { SystemPos.x + ADD_VEC[i].x, SystemPos.y + ADD_VEC[i].y };
					//空白では無い場合
					if (gNowMap[systemPos.y][systemPos.x] >= 0)
					{
						SetBallomMoveDir(base); //移動方向変更
						break;
					}
				}
			}

			int randomNum = Range_Random_Number(1, 10);
			if (randomNum <= 2)
			{
				SetBallomMoveDir(base); //移動方向変更
			}
		}
	}
}

//バロムの移動方向変更処理
void CBallom::SetBallomMoveDir(vector<unique_ptr<BaseVector>>& base)
{
	vector<int> moveDir;

	//プレイヤーを取得
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return;

	//爆弾を置いているかの情報を保存
	bool isKeepPutBomb = p->IsPutBomb;

	//空白のマスを moveDir に追加
	if (SystemPos.x < MAP_CHIP_W - 1 && SystemPos.x > 0 &&
		SystemPos.y < MAP_CHIP_H - 1 && SystemPos.y > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			MapPoint systemPos = { SystemPos.x + ADD_VEC[i].x, SystemPos.y + ADD_VEC[i].y };
			if (gNowMap[systemPos.y][systemPos.x] == -1)
				moveDir.push_back(i);
		}
	}

	//全て行き止まりの場合は終了
	if (moveDir.size() <= 0) {
		vec.x = 0.0f;
		vec.y = 0.0f;
		return;
	}
	//1つしか方向がない場合はその方向に設定
	else if (moveDir.size() == 1){

		//保存している爆弾を置いたフラグとプレイヤーの爆弾を置いたフラグが違っていた場合
		if (isKeepPutBomb != p->IsPutBomb) {
			SetBallomMoveDir(base);
			return;
		}
		vec.x = ADD_VEC[moveDir[0]].x * BALLOM_SPEED;
		vec.y = ADD_VEC[moveDir[0]].y * BALLOM_SPEED;
		return;
	}
	//1つ以上の場合
	else
	{
		int randomNum = Range_Random_Number(0, moveDir.size() - 1); //ランダム

		Vector moveVec = { ADD_VEC[moveDir[randomNum]].x * BALLOM_SPEED ,
						   ADD_VEC[moveDir[randomNum]].y * BALLOM_SPEED
		};

		//縦移動中
		if (vec.x == 0.0f && vec.y != 0.0f)
		{
			vector<int> priorityDir;
			for (int i = 0; i < moveDir.size(); i++)
			{
				if (moveDir[i] == BallomDir::LEFT || moveDir[i] == BallomDir::RIGHT)
					priorityDir.push_back(moveDir[i]);
			}
			//1つしか方向がない場合はその方向に設定
			if (priorityDir.size() == 1) {
				moveVec = { ADD_VEC[priorityDir[0]].x * BALLOM_SPEED,
							ADD_VEC[priorityDir[0]].y * BALLOM_SPEED
				};
			}
			//2つの場合はランダム
			if (priorityDir.size() == 2) {
				int randomPriorityNum = Range_Random_Number(0, 1);
				moveVec = { ADD_VEC[priorityDir[randomPriorityNum]].x * BALLOM_SPEED,
							ADD_VEC[priorityDir[randomPriorityNum]].y * BALLOM_SPEED
				};
			}
		}
		//横移動中
		else if (vec.y == 0.0f && vec.x != 0.0f)
		{
			vector<int> priorityDir;
			for (int i = 0; i < moveDir.size(); i++)
			{
				if (moveDir[i] == BallomDir::UP || moveDir[i] == BallomDir::DOWN)
					priorityDir.push_back(moveDir[i]);
			}
			//1つしか方向がない場合はその方向に設定
			if (priorityDir.size() == 1) {
				moveVec = { ADD_VEC[priorityDir[0]].x * BALLOM_SPEED,
							ADD_VEC[priorityDir[0]].y * BALLOM_SPEED
				};
			}
			//2つの場合はランダム
			if (priorityDir.size() == 2) {
				int randomPriorityNum = Range_Random_Number(0, 1);
				moveVec = { ADD_VEC[priorityDir[randomPriorityNum]].x * BALLOM_SPEED,
							ADD_VEC[priorityDir[randomPriorityNum]].y * BALLOM_SPEED
				};
			}
		}

		//保存している爆弾を置いたフラグとプレイヤーの爆弾を置いたフラグが違っていた場合
		if (isKeepPutBomb != p->IsPutBomb) {
			SetBallomMoveDir(base);
			return;
		}
		vec = moveVec;
	}
}

//バロムアニメーション処理
void CBallom::BallomAnim(int animMax, int* index, bool loop)
{
	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < BALLOM_ANIM_FRAME)
	{
		AnimCnt++;//インクリメント
		return;
	}
	//初期化
	AnimCnt = 0;

	//アニメーションの最大値以上の場合は初期化
	if (*index >= animMax - 1)
	{
		if(loop)
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

//死亡時の設定
void CBallom::EnemyDead()
{
	IsDead = true;
	AnimIndex = 3;
	AnimCnt = 0;
	vec.x = 0.0f;
	vec.y = 0.0f;
}