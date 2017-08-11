/**
 * @file LineTrace.c
 * @brief ���C���g���[�X�iPID����j
 * @author Keishi MURAI
 * @date 2017/06/19
 */
#include "LineTrace.h"

/**
 * @brief ���C���g���[�X����
 * @param [in] int targetVal �ڕW�̌��Z���T�l
 * @param [in] int currentVal ���݂̌��Z���T�l
 * @param [in] float opePeriod ��������[s]
 * @param [in,out] int errList[] �΍�����
 * @param [in,out] int nextErrIndex ���̕΍������C���f�b�N�X
 * @return ����p�x[deg.] 100(�E����ő�l)�`-100(������ő�l)
 * @detail ���Z���T�l�����PID������s��
 */
float LineTrace(int targetVal, int currentVal, float opePeriod, int errList[], int nextErrIndex) {
	int err;	//�΍�
	int i;	//�΍�����p�C���f�b�N�X
	int errNum;	//�΍�����
	int lastErrIndex;	//�O��΍��̃C���f�b�N�X
	int integral;	//�΍��ϕ�
	int diff;	//�΍�����
	float turn;	//����p�x

	// P����
	err = currentVal - targetVal;	// �����̍������g���[�X

	// I����
	errList[nextErrIndex] = err;
	errNum = sizeof errList / sizeof errList[0];
	integral = 0;
	for (i = 0; i < errNum; i++) {
		integral += errList[i] * opePeriod;
	}

	// D����
	if (nextErrIndex != 0) {
		lastErrIndex = nextErrIndex - 1;
	} else {
		lastErrIndex = errNum - 1;
	}
	diff = (err - errList[lastErrIndex]) / opePeriod;

	turn = KP * (float)err + KI * (float)integral + KD * (float)diff;

	if (nextErrIndex + 1 < errNum) {
		nextErrIndex++;
	} else {
		nextErrIndex = 0;
	}
	
	// ����p�x���͈͓��Ɏ��܂��Ă��邩�m�F
	if (turn > MAX_TURN_RIGHT) {
		turn = MAX_TURN_RIGHT;
	}
	else if (turn < MAX_TURN_LEFT) {
		turn = MAX_TURN_LEFT;
	}
	return turn;
}
