///////////////////////////////////////////////////////////
//  Motor.h
//  Implementation of the Class Motor
//  Created on:      2016/07/26 18:58:12
//  Original author: 039389
///////////////////////////////////////////////////////////

#if !defined(EA_5077E0BD_D409_458e_B98B_77B99C1975F0__INCLUDED_)
#define EA_5077E0BD_D409_458e_B98B_77B99C1975F0__INCLUDED_

#include "ev3api.h"

class Motor
{

public:
	Motor(motor_port_t PortNum);
	virtual ~Motor();

	void UpdateAngularVelocity();
	void ResetEnc();
	int GetEnc();
	int GetAngularVelocity();
	void SetPWMValue(int PWMValue);

private :
	motor_port_t m_Port;						// ポート番号
	int m_AngularVelocity;						// 角速度[deg/sec]
	int* m_MotorEncBuf;							// 角速度算出用エンコーダバッファ
	unsigned char m_MotorEncBufNextIndex;		// 角速度算出用エンコーダバッファ格納位置

};
#endif // !defined(EA_5077E0BD_D409_458e_B98B_77B99C1975F0__INCLUDED_)
