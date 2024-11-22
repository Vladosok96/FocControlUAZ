#include "Brake.h"
#include "Gearbox.h"
#include "motors.h"
#include "esp_log.h"

#define TAG "Brake"

float brakeMotorTorque = 4;

void setBrakeTorque(float brakeTorque) {
    if (brakeTorque <= 0) {
        brakeTorque = -1.5;
    }
    if (brakeTorque > 1) {
        brakeTorque = 6;
    }
    else {
        brakeTorque = 3 + brakeTorque * 3;
    }
    if (gearbox_state == GM_READY) {
        ESP_LOGW(TAG, "Setting brake trq = %f", brakeTorque);
        brakeMotorTorque = brakeTorque;
    } else {
        ESP_LOGW(TAG, "Gearbox busy, brake trq = %f", brakeTorque);
        lastBrakeTorque = brakeTorque;
    }
}

