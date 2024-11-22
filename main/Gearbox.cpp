#include "Gearbox.h"
#include "Brake.h"
#include "motors.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#define TAG "Gearbox"

int target_y = GM_P;
int last_target_y = GM_P;
int gearbox_state = GM_INIT;
int target_gear = 0;
float lastBrakeTorque = 5;


void set_gear(int target) {
    ESP_LOGI(TAG, "target gear: %d", target);
    if (gearbox_state != GM_READY) {
        ESP_LOGW(TAG, "Abort! Gearbox not ready");
        return;
    }
    if (target_gear == target) {
        // ESP_LOGW(TAG, "Abort! Already set");
        return;
    }
    last_target_y = target_y;
    target_gear = target;
    if (target == 0) {
        target_y = GM_P;
    }
    else if (target == 1) {
        target_y = GM_R;
    }
    else if (target == 2) {
        target_y = GM_N;
    }
    else if (target == 3) {
        target_y = GM_D;
    }

    gearbox_state = GM_MOVE;
}

void lockBrake() {
    lastBrakeTorque = brakeMotorTorque;
    brakeMotorTorque = 6;
    vTaskDelay(1500 / portTICK_PERIOD_MS);
}

void unlockBrake() {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    brakeMotorTorque = lastBrakeTorque;
}

void gearbox_task(void* args) {
    while (true) {
        if (gearbox_state == GM_INIT) {
            lockBrake();
            motorBox.target = motorBox.position_p;
            motorBox.enable();
            while (motorBox.getPosition() != GM_P) vTaskDelay(10 / portTICK_PERIOD_MS);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            motorBox.disable();
            unlockBrake();
            gearbox_state = GM_READY;
        }
        else if (gearbox_state == GM_MOVE) {
            if (motorBox.getPosition() == target_y) {
                gearbox_state = GM_READY;
            }
            else {
                lockBrake();
                motorBox.target = motorBox.getFloatPosition(target_y);
                motorBox.enable();
                int64_t begin_time = esp_timer_get_time();
                while (motorBox.getPosition() != target_y) {
                    if (esp_timer_get_time() - begin_time > 2000000) {
                        motorBox.target = motorBox.getFloatPosition(last_target_y);
                        target_y = last_target_y;
                    }
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                }
                vTaskDelay(500 / portTICK_PERIOD_MS);
                motorBox.disable();
                unlockBrake();
                gearbox_state = GM_READY;
            }
        }
        
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
}
