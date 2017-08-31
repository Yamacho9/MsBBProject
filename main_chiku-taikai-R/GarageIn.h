//******
// GarageIn.h
// 
// 
//******

#ifndef _GARAGEIN_H_INCLUDED
#define _GARAGEIN_H_INCLUDED

#include "ev3api.h"
#include "app.h"
#include "balancer.h"
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"

#define ENDGarageL 1070
#define ENDGarageR 794
#define DistMargineGarage 10

using namespace ev3api;

extern Mode GarageIn(int min, int max, ev3api::ColorSensor* colorSensor,ev3api::Motor* leftMotor, ev3api::Motor* rightMotor, ev3api::GyroSensor* gyroSensor, ev3api::Motor* tailMotor, ev3api::TouchSensor* touchSensor, ev3api::Clock* clock);

#endif //_GARAGEIN_H_INCLUDED
