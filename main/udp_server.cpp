#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_eth.h"
#include "driver/gpio.h"
#include "Ethernet.h"
#include "nvs_flash.h"
#include "lwip/sockets.h"
#include "jsonrpc-lean/server.h"
#include "esp_timer.h"

#include "Gearbox.h"
#include "Brake.h"

#include "udp_server.h"

#define PORT 90
#define TAG "udp_server"

jsonrpc::Server rpc_server;
jsonrpc::JsonFormatHandler jsonFormatHandler;
jsonrpc::Dispatcher *dispatcher;

int64_t udpLastCommandTime = 0;

void set_values(int32_t targetGear, double brakeTorque) {
    udpLastCommandTime = esp_timer_get_time();
    set_gear(targetGear);
    setBrakeTorque(brakeTorque);
}

void udp_server_task(void* arg)
{
    char rx_buffer[128];
    char addr_str[128];
    struct sockaddr_in dest_addr;

    while (1) {

        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(PORT);


        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", IPPROTO_IP);
            break;
        }
        ESP_LOGI(TAG, "Socket created");


        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(TAG, "Socket bound, port %d", PORT);

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t socklen = sizeof(source_addr);

        ESP_LOGI(TAG, "Waiting for data");

        while (1) {
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            if (len < 0) {
            }
            else {
                inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
                rx_buffer[len] = 0;
                rpc_server.HandleRequest(rx_buffer);
            }
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

void start_udp_server() {
    rpc_server.RegisterFormatHandler(jsonFormatHandler);
    dispatcher = &rpc_server.GetDispatcher();

    dispatcher->AddMethod("set_values", &set_values);

    xTaskCreatePinnedToCore(udp_server_task, "udp_server", 8192, NULL, 5, NULL, 0);
}