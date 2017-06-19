/**
 * @brief ���C���g���[�X����
 * @param [in] targetVal �ڕW�̌��Z���T�l
 * @param [in] currentVal ���݂̌��Z���T�l
 * @param [in] opePeriod ��������[s]
 * @param [in,out] errList �΍�����
 * @param [in,out] nextErrIndex ���̕΍������C���f�b�N�X
 * @return ����p�x[deg.] 100(�E����ő�l)�`-100(������ő�l)
 * @detail ���Z���T�l�����PID������s��
 */

#include "LineTrace.h"

#define KP					0.74 //���C���g���[�X���� ���W��
#define KI					0.01 //���C���g���[�X���� �ϕ��W��
#define KD					0.03 //���C���g���[�X���� �����W��

#define MAX_TURN_RIGHT	100
#define MAX_TURN_LEFT	-100

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
		integral += errorList[i] * opePeriod;
	}

	// D����
	nextErrIndex == 0 ? lastErrIndex = errNum - 1 : lastErrIndex = nextErrIndex - 1;
	diff = (err - errList[lastErrIndex]) / opePeriod;

	turn = KP * (float)error + KI * (float)integral + KD * (float)diff;

	nextErrIndex + 1 < errNum ? nextErrIndex++ : nextErrIndex = 0;

	if (turn > MAX_TURN_RIGHT) {
		turn = MAX_TURN_RIGHT;
	}
	else if (turn < MAX_TURN_LEFT) {
		turn = MAX_TURN_LEFT;
	}

}