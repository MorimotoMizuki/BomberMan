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
	gEnemyPri--;		//敵の描画順番
	gPlayerStatus.score += SCORE; //スコア加算
}

//敵の死亡時処理
void CBaseEnemy::EnemyDead(int ANIM_FRAME, int deadAnimFrame)
{
	if (!IsDead) return;

	if (!IsAddKillNum) {
		gKillEnemyNum++;	//敵討伐数++
		IsAddKillNum = true;
	}

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
				MapPoint bPos = (base[i].get())->SystemPos; //爆弾の座標
				if (bPos.x != SystemPos.x || bPos.y != SystemPos.y)
				{
					//その座標がブロックだった場合は終了
					if(CheckArea_of_SelectObj_Id(static_cast<Obj_Id>(gNowMap[SystemPos.y][SystemPos.x]), {Obj_Id::BLOCK, Obj_Id::CRASH_BLOCK, Obj_Id::BOMB}))
						return;

					//座標調整
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
void CBaseEnemy::RandomMove(vector<unique_ptr<BaseVector>>& base, const std::array<bool, 4>& dir, int change_dir_percentage)
{
	if (StopCnt >= STOP_FRAME) {

		//システム上の座標更新 : 左上座標から
		MapPoint systemPosL = { static_cast<int>((pos.x) / CHIP_SIZE) ,
								static_cast<int>(((pos.y) - WINDOW_HEADER) / CHIP_SIZE)
		};
		//システム上の座標更新 : 右下座標から
		MapPoint systemPosR = { static_cast<int>((pos.x + ImgWidth - 1   - (SPEED / 2)) / CHIP_SIZE) ,
								static_cast<int>(((pos.y + ImgHeight - 1 - (SPEED / 2)) - WINDOW_HEADER) / CHIP_SIZE)
		};

		if (systemPosL.x == systemPosR.x && systemPosL.y == systemPosR.y)
		{
			if(CheckMoveArea(SystemPos, HitMoveObj_Id))
			{
				Vector normalizeVec = Vector_Normalize(vec);
				bool isBreak = false;
				//ブロック, 爆弾の場合は移動方向変更
				for (int i = 0; i < 4; i++)
				{
					if (!dir[i]) continue;

					if (normalizeVec.x == ADD_VEC[i].x && normalizeVec.y == ADD_VEC[i].y)
					{
						MapPoint systemPos = { SystemPos.x + ADD_VEC[i].x, SystemPos.y + ADD_VEC[i].y };
						//ブロック, 爆弾だった場合 : ブロック　か　クラッシュブロック
						if(CheckArea_of_SelectObj_Id(static_cast<Obj_Id>(gNowMap[systemPos.y][systemPos.x]), {Obj_Id::BLOCK, Obj_Id::CRASH_BLOCK, Obj_Id::BOMB}))
						{
							SetMoveDir(base, dir); //移動方向変更
							StopCnt = 0;
							isBreak = true;
							break;
						}
					}
				}

				if (!isBreak) {
					int randomNum = Range_Random_Number(1, 10);
					if (randomNum <= change_dir_percentage)
					{
						SetMoveDir(base, dir); //移動方向変更
					}
				}
			}
		}
	}
	else
		StopCnt++;
}

//方向設定
void CBaseEnemy::SetMoveDir(vector<unique_ptr<BaseVector>>& base, const std::array<bool, 4>& dir)
{
	CPlayer* p = (CPlayer*)Get_obj(base, PLAYER);
	if (p == nullptr) return;

	vector<int> moveDir;
	//爆弾を置いているかの情報を保存
	bool isKeepPutBomb = p->IsPutBomb;

	//移動できるマスを moveDir に追加
	for (int i = 0; i < 4; i++)
	{
		if (!dir[i]) continue; //その方向に行く許可がない場合はスキップ

		MapPoint systemPos = { SystemPos.x + ADD_VEC[i].x, SystemPos.y + ADD_VEC[i].y };
		//移動できるか判定
		if (CheckMoveArea(systemPos, HitMoveObj_Id))
			moveDir.push_back(i);
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
			SetMoveDir(base, dir);
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
			SetMoveDir(base, dir);
			return;
		}
		vec = moveVec;
	}
}

//直線方向移動のフラグ変更処理
int CBaseEnemy::SetLineMoveIsDir(int dir_change_cnt, int STOP_FRAME)
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
		if (dir_change_cnt > STOP_FRAME) {

			if (IsPermitDir[MoveDir::LEFT]) {
				IsPermitDir[MoveDir::LEFT]	= false;
				IsPermitDir[MoveDir::RIGHT] = false;
				IsPermitDir[MoveDir::UP]	= true;
				IsPermitDir[MoveDir::DOWN]	= true;
			}
			else {
				IsPermitDir[MoveDir::LEFT]	= true;
				IsPermitDir[MoveDir::RIGHT] = true;
				IsPermitDir[MoveDir::UP]	= false;
				IsPermitDir[MoveDir::DOWN]	= false;
			}
			for (int i = 0; i < 4; i++) {
				if (!IsPermitDir[i]) continue;

				MapPoint mp = { SystemPos.x + ADD_VEC[i].x, SystemPos.y + ADD_VEC[i].y };
				if (mp.x < MAP_CHIP_W - 1 && mp.x > 0 &&
					mp.y < MAP_CHIP_H - 1 && mp.y > 0) {

					if (CheckMoveArea(mp, HitMoveObj_Id)){
						vec.x = ADD_VEC[i].x * SPEED;
						vec.y = ADD_VEC[i].y * SPEED;
						dir_change_cnt = 0;
						return dir_change_cnt;
					}
				}
			}
		}
		else
			dir_change_cnt++;
	}
	return dir_change_cnt;
}

//プレイヤー追跡処理
void CBaseEnemy::TrackingPlayerMove(CPlayer* p, float moveFrame, bool* isTrackingPlayer,std::pair<bool, int> tracking_parameter, int randomParameter)
{
	if (p == nullptr) return;

	//システム上の座標更新 : 左上座標から
	MapPoint systemPosL = { static_cast<int>((pos.x) / CHIP_SIZE) ,
							static_cast<int>(((pos.y) - WINDOW_HEADER) / CHIP_SIZE)
	};
	//システム上の座標更新 : 右下座標から
	MapPoint systemPosR = { static_cast<int>((pos.x + ImgWidth - 1 - (SPEED / 2)) / CHIP_SIZE) ,
							static_cast<int>(((pos.y + ImgHeight - 1 - (SPEED / 2)) - WINDOW_HEADER) / CHIP_SIZE)
	};

	if ((systemPosL.x == systemPosR.x && systemPosL.y == systemPosR.y) ||
		(systemPosL.x == systemPosR.x && systemPosL.y == systemPosR.y && move_cnt >= moveFrame))
	{
		//プレイヤーと同じマスの場合
		if ((p->SystemPos.x == systemPosL.x && p->SystemPos.y == systemPosL.y) ||
			(p->SystemPos.x == systemPosR.x && p->SystemPos.y == systemPosR.y) ||
			(p->SystemPos.x == SystemPos.x && p->SystemPos.y == SystemPos.y)) {
			vec.x = 0.0f;
			vec.y = 0.0f;
			*isTrackingPlayer = false;
			return;
		}

		vec_last_route.clear();

		Cell goal{ p->SystemPos.x, p->SystemPos.y };
		Cell start{ SystemPos.x, SystemPos.y };

		MapPoint check_goal{ goal.X, goal.Y };
		MapPoint check_start{ start.X, start.Y };

		if (CheckOutsideRangeArea(check_goal) && CheckOutsideRangeArea(check_start))
		{
			auto v_map = ArrToVec(gNowMap);
			//経路の計算
			list<Cell> last_route = ROUTE_CALCULATION2(MAP_CHIP_W, MAP_CHIP_H, start, goal, v_map, HitMoveObj_Id);

			for (auto& x : last_route) {
				vec_last_route.push_back(x);
				if (vec_last_route.size() > tracking_parameter.second) //tracking_distanceを超えたら break する
					break;
			}
		}

		//ルートが2つ以上 かつ　追跡距離範囲内　の場合
		if (vec_last_route.size() >= 2)
		{
			if ((tracking_parameter.first == true && vec_last_route.size() <= tracking_parameter.second) ||
				(tracking_parameter.first == false))
			{
				int randomNum = Range_Random_Number(1, 10);
				if (randomNum <= randomParameter)
				{
					MapPoint first = { vec_last_route[1].X,vec_last_route[1].Y };
					if (CheckMoveArea(first, HitMoveObj_Id))
					{
						Vector e_v = { first.x - SystemPos.x, first.y - SystemPos.y };
						e_v = Vector_Normalize(e_v);

						vec.x = e_v.x * SPEED;
						vec.y = e_v.y * SPEED;

						*isTrackingPlayer = true;
						move_cnt = 0.0f;
						return;
					}
				}
				else
				{
					*isTrackingPlayer = false;
					vec.x = 0.0f;
					vec.y = 0.0f;
				}
			}
			else
				*isTrackingPlayer = false;
		}
		else
			*isTrackingPlayer = false;
	}
	if (*isTrackingPlayer == false) {
		if (move_cnt < moveFrame)
			move_cnt += 1.0f;
	}
}

//スコア表示処理
void CBaseEnemy::DrawScore()
{
	DrawFormatString(pos.x - Distance + 30.0f, pos.y + 30.0f, GetColor(255, 255, 255), "%d", SCORE);
}

//マップ範囲内か判定する
bool CBaseEnemy::CheckOutsideRangeArea(MapPoint system_pos)
{
	//マップ範囲内
	if (system_pos.x < MAP_CHIP_W && system_pos.x >= 0 &&
		system_pos.y < MAP_CHIP_H && system_pos.y >= 0)
	{
		return true;
	}
	else
		return false;
}

//移動できるマスか判定する
bool CBaseEnemy::CheckMoveArea(MapPoint system_pos, std::vector<Obj_Id> hit_objId)
{
	if (hit_objId.empty()) return false;

	//マップ範囲内
	if (CheckOutsideRangeArea(system_pos))
	{
		for (int i = 0; i < hit_objId.size(); i++) {

			if (gNowMap[system_pos.y][system_pos.x] == static_cast<int>(hit_objId[i]))
				return false;
		}
		return true;
	}
	//マップ範囲外
	else
		return false;
}

//設定したObj_Idか判定する
bool CBaseEnemy::CheckArea_of_SelectObj_Id(Obj_Id obj_id, std::initializer_list<Obj_Id> obj_id_array)
{
	for (auto id : obj_id_array)
	{
		if (obj_id == id)
			return true;
	}
	return false;
}