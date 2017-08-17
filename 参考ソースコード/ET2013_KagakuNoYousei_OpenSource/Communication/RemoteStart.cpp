#include "Communication/RemoteStart.h"
#include "Bluetooth.h"

using namespace ecrobot;

RemoteStart::RemoteStart(Bluetooth* pBluetooth)
	:m_pBluetooth(pBluetooth),CMD_START('1') {

}

RemoteStart::~RemoteStart() {
}

//*****************************************************************************
// �֐��� : remote_start
// ���� : ����
// �Ԃ�l : 1(�X�^�[�g)/0(�ҋ@)
// �T�v : Bluetooth�ʐM�ɂ�郊���[�g�X�^�[�g�B Tera Term�Ȃǂ̃^�[�~�i���\�t�g����A
//       ASCII�R�[�h��1�𑗐M����ƁA�����[�g�X�^�[�g����B
//*****************************************************************************
bool RemoteStart::remote_start()
{
	int i;
	unsigned int rx_len;
	char rx_buf[BT_MAX_RX_BUF_SIZE];
	bool start = false;

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++) {
		rx_buf[i] = 0;					// ��M�o�b�t�@���N���A����
	}

	rx_len = m_pBluetooth->receive(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0) {
		// ��M�f�[�^����
		if (rx_buf[0] == CMD_START) {
			start = true;				// ��M�f�[�^=='1' �� �߂�l��true��ݒ肷��
		}
		m_pBluetooth->send(rx_buf, 0, rx_len);	// ��M�f�[�^���G�R�[�o�b�N����
	}

	return start;
}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

