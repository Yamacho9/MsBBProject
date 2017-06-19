/**
* @file CalcDistanceAndDirection.c
* @brief ���s�����ƌ����̎Z�o
* @author Keishi MURAI
* @date 2017/06/20
*/

#include "CalcDistanceAndDirection.h"

/**
* @brief ���s�����ƌ����̎Z�o
* @param [in] int32_t leftCnt ���ԗփ��[�^�[�̃G���R�[�_�[�l
* @param [in] int32_t rightCnt �E�ԗփ��[�^�[�̃G���R�[�_�[�l
* @param [out] int32_t distance ���s����
* @param [out] int32_t direction ����
* @return ����
* @detail ���E���[�^�[�̃G���R�[�_�[�̕��ϒl���狗���ƌ������Z�o
*/
void CalcDistanceAndDirection (int32_t leftCnt, int32_t rightCnt, int32_t distance, int32_t direction) {
	int32_t angle;
	angle = (leftCnt + rightCnt) / 2;	// ���[�^�[�̌��o�p�x�i�ݐϒl�j
	distance = angle * PI * DIAMETER / 360;		// ����
	// �����i�X�^�[�g���̌�����0�x�Ƃ��āA���v���̊p�x�j
	direction = (motor_ang_l % (360 * 4) - motor_ang_r % (360 * 4)) / 4; 
}
