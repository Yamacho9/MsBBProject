/**
 * @file LineTrace.h
 * @brief ���C���g���[�X�iPID����j
 * @author Keishi MURAI
 * @date 2017/06/19
 */

#include "LineTrace_param.h"

#ifndef _LINETRACE_H_INCLUDED
#define _LINETRACE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define KP	0.74 //���C���g���[�X���� ���W��
#define KI	0.01 //���C���g���[�X���� �ϕ��W��
#define KD	0.03 //���C���g���[�X���� �����W��

#define MAX_TURN_RIGHT	100 //�E����ő�l
#define MAX_TURN_LEFT	-100 //������ő�l

extern float LineTrace(enum Line_status status, int targetVal, int currentVal, float opePeriod, int errList[], int nextErrIndex, int8_t* forward);

#ifdef __cplusplus
}
#endif

#endif /* ! _LINETRACE_H_INCLUDED */