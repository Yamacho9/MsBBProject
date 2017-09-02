/**
 * @file LineTrace.c
 * @brief ���C���g���[�X�iPD����j
 * @author Keishi MURAI
 * @date 2017/06/19
 */
#include "LineTrace.h"

//#define DEBUG
#define FORWARD_DEBUG 30

/**
 * @brief ���C���g���[�X����
 * @param [in] int state ���C���̏��(����/�J�[�u)
 * @param [in] int targetVal �ڕW�̌��Z���T�l
 * @param [in] int currentVal ���݂̌��Z���T�l
 * @param [in] float opePeriod ��������[s]
 * @param [in,out] int* lastErr �O��΍�
 * @param [in, out] int forward
 * @param [out] int* errParam �΍��i�f�o�b�O�o�͗p�j
 * @param [out] int* diffParam �΍������i�f�o�b�O�o�͗p�j
 * @return ����p�x[deg.] 100(�E����ő�l)�`-100(������ő�l)
 * @detail ���Z���T�l�����PD������s��
 */
float LineTrace(int status, int targetVal, int currentVal, float opePeriod, int* lastErr, int8_t* forward, int* errParam, float* diffParam) {
	int err;	//�΍�
	int integral=0;	//�΍��ϕ�
	float diff=0.0;	//�΍�����
	float turn;	//����p�x
	
#ifndef DEBUG
	*forward = FORWARD;	// ������2�ł�"100"���Z�b�g
#else
	*forward = FORWARD_DEBUG;	// �T���v���R�[�X�p
#endif
	
	// P����
	err = currentVal - targetVal;	// �����̍������g���[�X

	// D����
	diff = (err - *lastErr) / opePeriod;
	*lastErr = err;

	turn = KP * (float)err + KI * (float)integral + KD * diff;

	// ����p�x���͈͓��Ɏ��܂��Ă��邩�m�F
	if (turn > MAX_TURN_RIGHT) {
		turn = MAX_TURN_RIGHT;
	}
	else if (turn < MAX_TURN_LEFT) {
		turn = MAX_TURN_LEFT;
	}
	*errParam = err;
	*diffParam = diff;
	return turn;
}
