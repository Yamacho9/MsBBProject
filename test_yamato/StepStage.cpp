/**
 ******************************************************************************
 ** �t�@�C���� : StepStage.cpp
 **
 ** �T�v : �K�i�ʉߍU���p�̃v���O����
 **
 ** ���L : �������
 ******************************************************************************
 **/


#include "StepStage.hpp"

/*
**! @note 
**! @param 
**! @return Ture:�K�i���� False:�K�i�������Ă��Ȃ�
*/
bool findStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak)
{
	bool right_frag = false;
	bool left_frag = false;
	
	bool ret = false;
	
	if((motor_ang_r - motor_ang_r_bak) < 0){
		right_frag = true;
	}
	if((motor_ang_l - motor_ang_l_bak) < 0){
		left_frag = true;
	}
	
	if(right_frag == true && left_frag == true){
		ret = true;
	}
	
	return ret;
}
/*
**! @note 
**! @param 
**! @return Ture:�K�i�N���A False:�K�i�N���A���Ă��Ȃ�
*/
bool clearStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak)
{
	bool right_frag = false;
	bool left_frag = false;
	
	bool ret = false;
	
	if((motor_ang_r - motor_ang_r_bak) > 2){
		right_frag = true;
	}
	if((motor_ang_l - motor_ang_l_bak) > 2){
		left_frag = true;
	}
	
	if(right_frag == true && left_frag == true){
		ret = true;
	}
	
	return ret;
}