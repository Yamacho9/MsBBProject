//******
// Calibration.h
// 
// 
//******

#include "ev3api.h"
#include "app.h"
#include "balancer.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"

using namespace ev3api;

#define GYRO_OFFSET_CALIBRATION   0  /*キャリブレーション時のジャイロオフセット値*/

void Calibration(int* min, int* max, ev3api::ColorSensor* colorSensor, ev3api::Motor* left, ev3api::Motor* right, ev3api::GyroSensor* gyroSen, ev3api::Motor* tail, ev3api::TouchSensor* touch, ev3api::Clock* clock);
bool tail_control_cal(int32_t angle, ev3api::Motor* tail, tailSpeed sp);
