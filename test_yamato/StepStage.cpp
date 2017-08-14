/**
 ******************************************************************************
 ** ファイル名 : StepStage.cpp
 **
 ** 概要 : 階段通過攻略用のプログラム
 **
 ** 注記 : 結合作業
 ******************************************************************************
 **/


#include "StepStage.hpp"


static bool ret_motor = false;
static bool ret_gyro = false;
static int count_r = 0;
static int count_l = 0;

/*
**! @note 
**! @param 
**! @return Ture:階段発見 False:階段見つかっていない
*/
bool findStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak,int32_t gyro)
{
	bool ret = false;	
	
	if(((motor_ang_r - motor_ang_r_bak) < 0) && ((motor_ang_l - motor_ang_l_bak) < 0)){
		//ret_motor = true;
		ret = true;
	}
	/*if(gyro > 150){
		ret_gyro = true;
	}
	
	if(ret_motor && ret_gyro){
		ret = true;
		ret_motor = false;
		ret_gyro = false;
	}*/
	
	return ret;
}


/*
**! @note しっぽ走行時用の段差検知
**! @param 
**! @return Ture:階段発見 False:階段見つかっていない
*/
bool findStep2(int32_t gyro)
{
	bool ret = false;	
	
	if(gyro>=15 || gyro<=-15){
		ret = true;
	}
	return ret;
}
/*
**! @note 
**! @param 
**! @return Ture:階段クリア False:階段クリアしていない
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