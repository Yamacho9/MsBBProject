/**
 * @file LineTrace_param.h
 * @brief ���C���g���[�X�iPID����j�̃p�����[�^�ǂݍ��݂̃w�b�_�t�@�C��
 * @author kanako baba ww
 * @date 2017/07/23
 */
#include "ev3api.h"
#include "app.h"

/* �p�����[�^�̃f�t�H���g�l�i���s���Ɏg�p�j */
#define D_FORWARD 50
#define D_KP 0.74
#define D_KI 0.01
#define D_KD 0.03

enum Line_status{
	SECTION1,
	SECTION2,
	SECTION3,
	SECTION4,
	SECTION5,
	SECTION6,
	SECTION7,
	SECTION8,
	SECTION9,
	SECTION_DEFAULT,

};	

extern void LineTrace_param(enum Line_status status, int8_t* forward, float* kp, float* ki,float* kd);
