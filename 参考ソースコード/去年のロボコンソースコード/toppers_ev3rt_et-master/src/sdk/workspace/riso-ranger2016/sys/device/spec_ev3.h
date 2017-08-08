///////////////////////////////////////////////////////////
//  spec_ev3.h
//  Implementation of the Class spec_ev3
//  Created on:      2016/05/23 18:20:06
//  Original author: 037789
///////////////////////////////////////////////////////////

#if !defined(EA_142A24DA_76BC_4b8f_800D_658F4C509536__INCLUDED_)
#define EA_142A24DA_76BC_4b8f_800D_658F4C509536__INCLUDED_

#include "ev3api.h"


// -----------------------------------------------
//! @struct T_SensorConfig
//! @brief  センサコンフィグ用構造体.
//!
//! EV3 のセンサー入力ポートの設定用の構造体.
// -----------------------------------------------
typedef struct _T_SensorConfig {
	sensor_port_t port;		//!< センサポート番号.
	sensor_type_t type;		//!< センサタイブ.
} T_SensorConfig;

extern const T_SensorConfig st_sensor_config_tbl[];


// -----------------------------------------------
//! @struct T_MotorConfig
//! @brief  モータコンフィグ用構造体.
//!
//! EV3 のモータ出力ポートの設定用の構造体.
// -----------------------------------------------
typedef struct _T_MotorConfig {
	motor_port_t port;		//!< センサポート番号.
	motor_type_t type;		//!< センサタイブ.
} T_MotorConfig;

extern const T_MotorConfig st_motor_config_tbl[];

class spec_ev3
{

public:
	spec_ev3();
	virtual ~spec_ev3();

};
#endif // !defined(EA_142A24DA_76BC_4b8f_800D_658F4C509536__INCLUDED_)
