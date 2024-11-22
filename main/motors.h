#pragma once
#include "SimpleFOC.h"

extern BLDCMotor motorBox;
extern BLDCMotor motorBrake;

extern MagneticSensorSSC sensorBox;
extern MagneticSensorSSC sensorBrake;

void initMotorsSpi();
void initMotorBrake();
void initMotorBox();