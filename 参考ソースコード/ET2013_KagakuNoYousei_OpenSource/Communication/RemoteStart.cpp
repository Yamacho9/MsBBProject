#include "Communication/RemoteStart.h"
#include "Bluetooth.h"

using namespace ecrobot;

RemoteStart::RemoteStart(Bluetooth* pBluetooth)
	:m_pBluetooth(pBluetooth),CMD_START('1') {

}

RemoteStart::~RemoteStart() {
}

//*****************************************************************************
// 関数名 : remote_start
// 引数 : 無し
// 返り値 : 1(スタート)/0(待機)
// 概要 : Bluetooth通信によるリモートスタート。 Tera Termなどのターミナルソフトから、
//       ASCIIコードで1を送信すると、リモートスタートする。
//*****************************************************************************
bool RemoteStart::remote_start()
{
	int i;
	unsigned int rx_len;
	char rx_buf[BT_MAX_RX_BUF_SIZE];
	bool start = false;

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++) {
		rx_buf[i] = 0;					// 受信バッファをクリアする
	}

	rx_len = m_pBluetooth->receive(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0) {
		// 受信データあり
		if (rx_buf[0] == CMD_START) {
			start = true;				// 受信データ=='1' ⇒ 戻り値にtrueを設定する
		}
		m_pBluetooth->send(rx_buf, 0, rx_len);	// 受信データをエコーバックする
	}

	return start;
}


//Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

