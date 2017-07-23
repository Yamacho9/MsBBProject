/**
 * @file LineTrace_param.h
 * @brief ���C���g���[�X�iPID����j�̃p�����[�^�ǂݍ��݂̃w�b�_�t�@�C��
 * @author kanako baba ww
 * @date 2017/07/23
 */
#include <stdio.h>
#include "ev3api.h"

/* �p�����[�^�̃f�t�H���g�l�i���s���Ɏg�p�j */
#define D_FORWARD 50
#define D_KP 0.74
#define D_KI 0.01
#define D_KD 0.03

enum Line_status{
	LINESTATUS_DEFAULT,
	LINESTATUS_STRAIGHT,
	LINESTATUS_CURVE_WEAK,
	LINESTATUS_CURVE_STRONG,
};	

extern int LineTrace_param(enum Line_status status, int8_t* forward, float* kp, float* ki,float* kd);
