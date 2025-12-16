#include"obj.h"
#include"function.h"

//コンストラクタ
void CBaseEnemy::Constructor(Point p, MapPoint system_p)
{
	img = LoadGraph("image\\enemy.png");

	//画像を分割 : 死亡時画像
	for (int i = 0; i < 4; i++) {
		EnemyDeadImgHandle[i + 1] = DerivationGraph(IMGSIZE32 * i, IMGSIZE32, IMGSIZE32, IMGSIZE32, img);
	}

	pos = p;

	SystemPos = system_p;

	ImgWidth  = CHIP_SIZE;
	ImgHeight = CHIP_SIZE;

	ID  = Obj_Id::ENEMY;
	pri = gEnemyPri;
	gEnemyPri++;
}

//デストラクタ
void CBaseEnemy::Destructor()
{
	gKillEnemyNum++;	//敵討伐数++
	gEnemyPri--;		//敵の描画順番

	gPlayerStatus.score += SCORE; //スコア加算
}

//敵の死亡時処理
void CBaseEnemy::EnemyDead(int ANIM_FRAME, int deadAnimFrame)
{
	if (!IsDead) return;

	if (DeadCnt < deadAnimFrame)
		DeadCnt++;
	else
	{
		if (!IsDrawScore) {
			if (Anim(ANIM_FRAME, 5, &AnimIndex, false))
				IsDrawScore = true;
		}
		else
		{
			if (AnimCnt > 60)
				FLAG = false;
			else
				AnimCnt++;
		}
	}
}

//爆弾と接触時の座標調整処理
void CBaseEnemy::HitBomb_PosAdjustment(vector<unique_ptr<BaseVector>>& base)
{
	//爆弾と接触時位置調整処理
	for (int i = 0; i < base.size(); i++)
	{
		//削除対象のオブジェクトはスキップ
		if (!base[i]->FLAG || !base[i]->draw_flag)
			continue;

		//爆弾オブジェクトと判定
		if (base[i]->ID == BOMB){
			if (HitCheck_box(this, base[i].get())){
				MapPoint bPos = (base[i].get())->SystemPos;
				if (bPos.x != SystemPos.x || bPos.y != SystemPos.y)
				{
					pos.x = (SystemPos.x * CHIP_SIZE);
					pos.y = (SystemPos.y * CHIP_SIZE) + WINDOW_HEADER;
					return;
				}
			}
		}
	}
}

//アニメーション処理
bool CBaseEnemy::Anim(int ANIM_FRAME, int animMax, int* index, bool loop)
{
	//アニメーションカウントが定数未満の場合は終了
	if (AnimCnt < ANIM_FRAME)
	{
		AnimCnt++;//インクリメント
		return false;
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
			return true;
		}
	}
	else
		*index += 1;

	return false;
}

//ランダム移動処理
void CBaseEnemy::RandomMove(vector<unique_ptr<BaseVector>>& base)
{
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return;

	if (StopCnt >= 5) {

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

				//ブロック, 爆弾の場合は移動方向変更
				for (int i = 0; i < 4; i++)
				{
					if (normalizeVec.x == ADD_VEC[i].x && normalizeVec.y == ADD_VEC[i].y)
					{
						MapPoint systemPos = { SystemPos.x + ADD_VEC[i].x, SystemPos.y + ADD_VEC[i].y };
						//ブロック, 爆弾だった場合 : ブロック　か　クラッシュブロック
						if (gNowMap[systemPos.y][systemPos.x] == BLOCK ||
							gNowMap[systemPos.y][systemPos.x] == BLOCK + 1 ||
							gNowMap[systemPos.y][systemPos.x] == BOMB)
						{
							SetMoveDir(base); //移動方向変更
							StopCnt = 0;
							break;
						}
					}
				}

				int randomNum = Range_Random_Number(1, 10);
				if (randomNum <= 2)
				{
					SetMoveDir(base); //移動方向変更
				}
			}
		}
	}
	else
		StopCnt++;
}

//方向設定
void CBaseEnemy::SetMoveDir(vector<unique_ptr<BaseVector>>& base)
{
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return;

	vector<int> moveDir;
	//爆弾を置いているかの情報を保存
	bool isKeepPutBomb = p->IsPutBomb;

	//空白のマスを moveDir に追加
	if (SystemPos.x < MAP_CHIP_W - 1 && SystemPos.x > 0 &&
		SystemPos.y < MAP_CHIP_H - 1 && SystemPos.y > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			MapPoint systemPos = { SystemPos.x + ADD_VEC[i].x, SystemPos.y + ADD_VEC[i].y };
			//ブロック, 爆弾以外の場合
			if (gNowMap[systemPos.y][systemPos.x] != BLOCK &&
				gNowMap[systemPos.y][systemPos.x] != BLOCK + 1 &&
				gNowMap[systemPos.y][systemPos.x] != BOMB)
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
	else if (moveDir.size() == 1) {

		//保存している爆弾を置いたフラグとプレイヤーの爆弾を置いたフラグが違っていた場合
		if (isKeepPutBomb != p->IsPutBomb) {
			SetMoveDir(base);
			return;
		}
		vec.x = ADD_VEC[moveDir[0]].x * SPEED;
		vec.y = ADD_VEC[moveDir[0]].y * SPEED;
		return;
	}
	//1つ以上の場合
	else
	{
		int randomNum = Range_Random_Number(0, moveDir.size() - 1); //ランダム

		Vector moveVec = { ADD_VEC[moveDir[randomNum]].x * SPEED ,
						   ADD_VEC[moveDir[randomNum]].y * SPEED
		};

		//縦移動中
		if (vec.x == 0.0f && vec.y != 0.0f)
		{
			vector<int> priorityDir;
			for (int i = 0; i < moveDir.size(); i++)
			{
				if (moveDir[i] == MoveDir::LEFT || moveDir[i] == MoveDir::RIGHT)
					priorityDir.push_back(moveDir[i]);
			}
			//1つしか方向がない場合はその方向に設定
			if (priorityDir.size() == 1) {
				moveVec = { ADD_VEC[priorityDir[0]].x * SPEED,
							ADD_VEC[priorityDir[0]].y * SPEED
				};
			}
			//2つの場合はランダム
			if (priorityDir.size() == 2) {
				int randomPriorityNum = Range_Random_Number(0, 1);
				moveVec = { ADD_VEC[priorityDir[randomPriorityNum]].x * SPEED,
							ADD_VEC[priorityDir[randomPriorityNum]].y * SPEED
				};
			}
		}
		//横移動中
		else if (vec.y == 0.0f && vec.x != 0.0f)
		{
			vector<int> priorityDir;
			for (int i = 0; i < moveDir.size(); i++)
			{
				if (moveDir[i] == MoveDir::UP || moveDir[i] == MoveDir::DOWN)
					priorityDir.push_back(moveDir[i]);
			}
			//1つしか方向がない場合はその方向に設定
			if (priorityDir.size() == 1) {
				moveVec = { ADD_VEC[priorityDir[0]].x * SPEED,
							ADD_VEC[priorityDir[0]].y * SPEED
				};
			}
			//2つの場合はランダム
			if (priorityDir.size() == 2) {
				int randomPriorityNum = Range_Random_Number(0, 1);
				moveVec = { ADD_VEC[priorityDir[randomPriorityNum]].x * SPEED,
							ADD_VEC[priorityDir[randomPriorityNum]].y * SPEED
				};
			}
		}

		//保存している爆弾を置いたフラグとプレイヤーの爆弾を置いたフラグが違っていた場合
		if (isKeepPutBomb != p->IsPutBomb) {
			SetMoveDir(base);
			return;
		}
		vec = moveVec;
	}
}

//プレイヤー追跡処理
void CBaseEnemy::TrackingPlayerMove(CPlayer* p, int moveFrame, bool* isTrackingPlayer)
{
	if (p == nullptr) return;

	//システム上の座標更新 : 左上座標から
	MapPoint systemPosL = { static_cast<int>((pos.x) / CHIP_SIZE) ,
							static_cast<int>(((pos.y) - WINDOW_HEADER) / CHIP_SIZE)
	};
	//システム上の座標更新 : 右下座標から
	MapPoint systemPosR = { static_cast<int>((pos.x + ImgWidth - 1) / CHIP_SIZE) ,
							static_cast<int>(((pos.y + ImgHeight - 1) - WINDOW_HEADER) / CHIP_SIZE)
	};

	if ((systemPosL.x == systemPosR.x && systemPosL.y == systemPosR.y) ||
		systemPosL.x == systemPosR.x && systemPosL.y == systemPosR.y && move_cnt > moveFrame)
	{
		if ((p->SystemPos.x == systemPosL.x && p->SystemPos.y == systemPosL.y) ||
			(p->SystemPos.x == systemPosR.x && p->SystemPos.y == systemPosR.y) ||
			(p->SystemPos.x == SystemPos.x && p->SystemPos.y == SystemPos.y)) {
			vec.x = 0.0f;
			vec.y = 0.0f;
			*isTrackingPlayer = false;
			return;
		}

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

			if (gNowMap[first.Y][first.X] != Obj_Id::NONE) {
				vec.x = 0.0f;
				vec.y = 0.0f;
				*isTrackingPlayer = false;
				return;
			}

			Vector e_v = { first.X - SystemPos.x, first.Y - SystemPos.y };
			e_v = Vector_Normalize(e_v);

			vec.x = e_v.x * SPEED;
			vec.y = e_v.y * SPEED;

			*isTrackingPlayer = true;
			move_cnt = 0;
			return;
		}
		else
		{
			*isTrackingPlayer = false;
		}
	}
	else
		move_cnt++;
}

//スコア表示処理
void CBaseEnemy::DrawScore()
{
	DrawFormatString(pos.x - Distance + 40.0f, pos.y + 40.0f, GetColor(255, 255, 255), "%d", SCORE);
}