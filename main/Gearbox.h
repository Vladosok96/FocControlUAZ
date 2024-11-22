#pragma once
#include "BLDCMotor.h"

extern int target_x;
extern int target_y;
extern int gearbox_state;
extern float lastBrakeTorque;


void set_gear(int target);
void gearbox_task(void* args);
