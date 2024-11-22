#include "nvstorage.h"

static const char *TAG = "NVS";

system_params systemParams;

brake_motor_params brakeMotorParams;

gearbox_motor_params gearboxMotorParams;


void nvs_init(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}

void loadSystemParams(){
    esp_err_t err;
    nvs_handle_t nvs_handler;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handler);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG,"Reading system params from NVS ...");

        nvs_get_i8(nvs_handler, "dhcp", (int8_t*)&systemParams.dhcp);
        nvs_get_i8(nvs_handler, "ip1", (int8_t*)&systemParams.ip[0]);
        nvs_get_i8(nvs_handler, "ip2", (int8_t*)&systemParams.ip[1]);
        nvs_get_i8(nvs_handler, "ip3", (int8_t*)&systemParams.ip[2]);
        nvs_get_i8(nvs_handler, "ip4", (int8_t*)&systemParams.ip[3]);
        nvs_get_i8(nvs_handler, "ma1", (int8_t*)&systemParams.mask[0]);
        nvs_get_i8(nvs_handler, "ma2", (int8_t*)&systemParams.mask[1]);
        nvs_get_i8(nvs_handler, "ma3", (int8_t*)&systemParams.mask[2]);
        nvs_get_i8(nvs_handler, "ma4", (int8_t*)&systemParams.mask[3]);
        nvs_get_i8(nvs_handler, "gw1", (int8_t*)&systemParams.gateway[0]);
        nvs_get_i8(nvs_handler, "gw2", (int8_t*)&systemParams.gateway[1]);
        nvs_get_i8(nvs_handler, "gw3", (int8_t*)&systemParams.gateway[2]);
        nvs_get_i8(nvs_handler, "gw4", (int8_t*)&systemParams.gateway[3]);

        nvs_close(nvs_handler);
    }
}

void saveSystemParams(){
    esp_err_t err;
    nvs_handle_t nvs_handler;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handler);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Saving system params to NVS ...");
        if (systemParams.dhcp == false){
            nvs_set_i8(nvs_handler, "ip1", systemParams.ip[0]);
            nvs_set_i8(nvs_handler, "ip2", systemParams.ip[1]);
            nvs_set_i8(nvs_handler, "ip3", systemParams.ip[2]);
            nvs_set_i8(nvs_handler, "ip4", systemParams.ip[3]);
            nvs_set_i8(nvs_handler, "ma1", systemParams.mask[0]);
            nvs_set_i8(nvs_handler, "ma2", systemParams.mask[1]);
            nvs_set_i8(nvs_handler, "ma3", systemParams.mask[2]);
            nvs_set_i8(nvs_handler, "ma4", systemParams.mask[3]);
            nvs_set_i8(nvs_handler, "gw1", systemParams.gateway[0]);
            nvs_set_i8(nvs_handler, "gw2", systemParams.gateway[1]);
            nvs_set_i8(nvs_handler, "gw3", systemParams.gateway[2]);
            nvs_set_i8(nvs_handler, "gw4", systemParams.gateway[3]);
        }
        nvs_set_i8(nvs_handler, "dhcp", systemParams.dhcp);

        nvs_commit(nvs_handler);
        nvs_close(nvs_handler);
    }
}

void loadBrakeMotorParams() {
    esp_err_t err;
    nvs_handle_t nvs_handler;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handler);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG,"Reading brake motor params from NVS ...");

        nvs_get_i8(nvs_handler, "BrCalibrate", (int8_t*)&brakeMotorParams.calibrate);
        nvs_get_i8(nvs_handler, "BrInverseEnc", (int8_t*)&brakeMotorParams.inverseEncoder);
        nvs_get_u32(nvs_handler, "BrEncAngle", (uint32_t*)&brakeMotorParams.encoderAngle);
        nvs_get_u32(nvs_handler, "BrVoltLimit", (uint32_t*)&brakeMotorParams.voltageLimit);

        nvs_close(nvs_handler);
    }
}

void saveBrakeMotorParams() {
    esp_err_t err;
    nvs_handle_t nvs_handler;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handler);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Saving brake motor params to NVS ...");
        nvs_set_i8(nvs_handler, "BrCalibrate", brakeMotorParams.calibrate);
        nvs_set_i8(nvs_handler, "BrInverseEnc", brakeMotorParams.inverseEncoder);
        nvs_set_u32(nvs_handler, "BrEncAngle", *(uint32_t*)(&brakeMotorParams.encoderAngle));
        nvs_set_u32(nvs_handler, "BrVoltLimit", *(uint32_t*)(&brakeMotorParams.voltageLimit));
        
        nvs_commit(nvs_handler);
        nvs_close(nvs_handler);
    }
}

void loadGearboxMotorParams() {
    esp_err_t err;
    nvs_handle_t nvs_handler;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handler);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG,"Reading gearbox motor params from NVS ...");

        nvs_get_i8(nvs_handler, "GbCalibrate", (int8_t*)&gearboxMotorParams.calibrate);
        nvs_get_i8(nvs_handler, "GbInverseEnc", (int8_t*)&gearboxMotorParams.inverseEncoder);
        nvs_get_u32(nvs_handler, "GbEncAngle", (uint32_t*)&gearboxMotorParams.encoderAngle);
        nvs_get_u32(nvs_handler, "GbVoltLimit", (uint32_t*)&gearboxMotorParams.voltageLimit);
        nvs_get_u32(nvs_handler, "GbVelLimitHard", (uint32_t*)&gearboxMotorParams.velLimitHard);
        nvs_get_u32(nvs_handler, "GbVelProp", (uint32_t*)&gearboxMotorParams.velProp);
        nvs_get_u32(nvs_handler, "GbVelIntegral", (uint32_t*)&gearboxMotorParams.velIntegral);
        nvs_get_u32(nvs_handler, "GbVelDiff", (uint32_t*)&gearboxMotorParams.velDiff);
        nvs_get_u32(nvs_handler, "GbVelRamp", (uint32_t*)&gearboxMotorParams.velRamp);
        nvs_get_u32(nvs_handler, "GbVelLimit", (uint32_t*)&gearboxMotorParams.velLimit);
        nvs_get_u32(nvs_handler, "GbVelFilter", (uint32_t*)&gearboxMotorParams.velFilter);
        nvs_get_u32(nvs_handler, "GbAngleProp", (uint32_t*)&gearboxMotorParams.angleProp);
        nvs_get_u32(nvs_handler, "GbAngleLimit", (uint32_t*)&gearboxMotorParams.angleLimit);

        nvs_close(nvs_handler);
    }
}

void saveGearboxMotorParams() {
    esp_err_t err;
    nvs_handle_t nvs_handler;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handler);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Saving gearbox motor params to NVS ...");
        nvs_set_i8(nvs_handler, "GbCalibrate", gearboxMotorParams.calibrate);
        nvs_set_i8(nvs_handler, "GbInverseEnc", gearboxMotorParams.inverseEncoder);
        nvs_set_u32(nvs_handler, "GbEncAngle", *(uint32_t*)(&gearboxMotorParams.encoderAngle));
        nvs_set_u32(nvs_handler, "GbVoltLimit", *(uint32_t*)(&gearboxMotorParams.voltageLimit));
        nvs_set_u32(nvs_handler, "GbVelLimitHard", *(uint32_t*)(&gearboxMotorParams.velLimitHard));
        nvs_set_u32(nvs_handler, "GbVelProp", *(uint32_t*)(&gearboxMotorParams.velProp));
        nvs_set_u32(nvs_handler, "GbVelIntegral", *(uint32_t*)(&gearboxMotorParams.velIntegral));
        nvs_set_u32(nvs_handler, "GbVelDiff", *(uint32_t*)(&gearboxMotorParams.velDiff));
        nvs_set_u32(nvs_handler, "GbVelRamp", *(uint32_t*)(&gearboxMotorParams.velRamp));
        nvs_set_u32(nvs_handler, "GbVelLimit", *(uint32_t*)(&gearboxMotorParams.velLimit));
        nvs_set_u32(nvs_handler, "GbVelFilter", *(uint32_t*)(&gearboxMotorParams.velFilter));
        nvs_set_u32(nvs_handler, "GbAngleProp", *(uint32_t*)(&gearboxMotorParams.angleProp));
        nvs_set_u32(nvs_handler, "GbAngleLimit", *(uint32_t*)(&gearboxMotorParams.angleLimit));
        
        nvs_commit(nvs_handler);
        nvs_close(nvs_handler);
    }
}