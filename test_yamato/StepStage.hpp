//******
// StepStage.h
// 
// 
//******

#include "ev3api.h"
#include "app.h"

extern bool findStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak,int32_t gyro);
extern bool findStep2(int32_t gyro);
extern bool clearStep(int32_t motor_ang_l,int32_t motor_ang_r,int32_t motor_ang_l_bak,int32_t motor_ang_r_bak);