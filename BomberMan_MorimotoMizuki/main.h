//DX Library Copyright (C) 2001-2025 Takumi Yamada.
//Dxlib_lib_ver6.02 2025 Kcom�e���v���[�g Shinya Wakisaka.
#pragma once
#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <list>
#include <vector>
#include <memory>
#include <time.h>

#include "objID.h"

using namespace std;

//�摜�T�C�Y
#define IMGSIZE64 64
#define IMGSIZE32 32
#define IMGSIZE16 16

//���W�A���ϊ��}�N��
#define RADIAN(_deg) ((float)M_PI*(_deg)/180.0f)
#define DEGREE(_rad) ((float)(_rad)*180.0f/M_PI)

constexpr auto WINDOW_HEADER = 96;	//��ʏ�̃T�C�Y

//��{�E�B���h�E�T�C�Y
constexpr auto WINDOW_WIDTH = 1024;	//���� 64*16
constexpr auto WINDOW_HEIGHT = IMGSIZE64 * 13 + WINDOW_HEADER;	//�c�� 64*13+48


//2D�p
 struct Point {
	float x, y;
};
 struct Vector {
	float x, y;
};

//3D�p
struct Point3 {
	float x, y, z;
};
struct Vector3 {
	float x, y, z;
};
 struct ObjSize {
	float h, w, d;
};

 struct PlayerStatus {
	 
	 //�ړ����x
	 float speed;
	 //�ݒu�\�Ȕ��e�̐�
	 int bombPutNum;
	 //���e�̈З͂̃��x��
	 int bombLevel;

 };

//�v���C���[�̃X�e�[�^�X
extern PlayerStatus gPlayerStatus;
//���ݐݒu���Ă��锚�e�̐�
extern int gNowBombNum;

//�J�������N���X(main.h)
//class CCameraInfo {
//private:
//	static Point3 pos;//�J�����̈ʒu
//public:
//	static Point3 GetPos() { return pos; }//�J�����̈ʒu���擾
//	static void SetPos(Point3 p) { pos = p; }//�J�����̈ʒu���ۑ�
//};