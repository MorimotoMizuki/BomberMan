#include"My_function.h"

//キー入力チェック関数
bool Key_Check(Move_Id move_id)
{
	//パッドの状態の取得
	int pad = GetJoypadInputState(DX_INPUT_PAD1);
	switch (move_id)
	{
	case UP:
		if (CheckHitKey(KEY_INPUT_UP) || (pad & PAD_INPUT_UP))
			return true;

		break;
	case RIGHT:
		if (CheckHitKey(KEY_INPUT_RIGHT) || (pad & PAD_INPUT_RIGHT))
			return true;

		break;
	case DOWN:
		if (CheckHitKey(KEY_INPUT_DOWN) || (pad & PAD_INPUT_DOWN))
			return true;

		break;
	case LEFT:
		if (CheckHitKey(KEY_INPUT_LEFT) || (pad & PAD_INPUT_LEFT))
			return true;

		break;
	case LEFT_CLICK:
		if ((GetMouseInput() & MOUSE_INPUT_LEFT))
			return true;

		break;
	case SPACE:
		if (CheckHitKey(KEY_INPUT_SPACE))
			return true;

		break;
	case ALL_KEY:
		if (CheckHitKeyAll() || pad)
			return true;

		break;
	}
	return false;
}