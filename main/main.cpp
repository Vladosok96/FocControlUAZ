#include <stdio.h>
#include "esp_log.h"
#include "SimpleFOC.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_eth.h"
#include "esp_timer.h"
#include "Ethernet.h"
#include "driver/spi_master.h"
#include "webserver.h"
#include "variables.h"
#include "nvstorage.h"
#include "motors.h"
#include "Gearbox.h"
#include "Brake.h"
#include "udp_server.h"

static const char* TAG = "main";

void safetyTask(void* args) {
    while(true) {
        if (esp_timer_get_time() - udpLastCommandTime > 500000) {
            set_gear(0);
            setBrakeTorque(1);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void motorTask(void* args) {
    int64_t high_current_time = esp_timer_get_time(); 

    while (true) {
        motorBox.loopFOC();
        motorBox.move();
        motorBrake.loopFOC();

        if (brakeMotorTorque < 5.5) {
            high_current_time = esp_timer_get_time();
        }

        if (esp_timer_get_time() - high_current_time > 5000000 && gearbox_state == GM_READY) {
            motorBrake.move(4);
        }
        else {
            motorBrake.move(brakeMotorTorque);
        }
    }
}


extern "C" void app_main(void) { 
    initMotorsSpi();
    nvs_init();
    loadSystemParams();
    vTaskDelay(20 / portTICK_PERIOD_MS);
    initMotorBrake();
    initMotorBox();

    setBrakeTorque(0.7);

    xTaskCreatePinnedToCore(motorTask, "Motor task", 8192, NULL, 99, NULL, 1);
    xTaskCreate(gearbox_task, "Gearbox task", 8192, NULL, 5, NULL);
    xTaskCreate(safetyTask, "Safety task", 8192, NULL, 10, NULL);


    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    Ethernet::init(40, 41, 2, 42, 1, SPI3_HOST);
    ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL));
    Ethernet::start();
    

    start_web_server();
    start_udp_server();
}