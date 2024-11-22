#include "web_funcs.h"
#include "motors.h"

static const char *TAG = "WebFuncs";


std::string webSystemJson() {
    std::string json = "{";
    json += "\"dhcp\":\"";
    json += (systemParams.dhcp == true ? "checked" : "0");

    json += "\",\"ip1\":\"";
    json += std::to_string(systemParams.ip[0]);
    json += "\",\"ip2\":\"";
    json += std::to_string(systemParams.ip[1]);
    json += "\",\"ip3\":\"";
    json += std::to_string(systemParams.ip[2]);
    json += "\",\"ip4\":\"";
    json += std::to_string(systemParams.ip[3]);

    json += "\",\"ma1\":\"";
    json += std::to_string(systemParams.mask[0]);
    json += "\",\"ma2\":\"";
    json += std::to_string(systemParams.mask[1]);
    json += "\",\"ma3\":\"";
    json += std::to_string(systemParams.mask[2]);
    json += "\",\"ma4\":\"";
    json += std::to_string(systemParams.mask[3]);

    json += "\",\"gw1\":\"";
    json += std::to_string(systemParams.gateway[0]);
    json += "\",\"gw2\":\"";
    json += std::to_string(systemParams.gateway[1]);
    json += "\",\"gw3\":\"";
    json += std::to_string(systemParams.gateway[2]);
    json += "\",\"gw4\":\"";
    json += std::to_string(systemParams.gateway[3]);

    json += "\"}";
    return json;
}

void webSystemSet(httpd_req_t *req) {
    size_t buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        char*  buf;
        buf = (char*)malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[50];
            if (httpd_query_key_value(buf, "dhcp", param, sizeof(param)) == ESP_OK) {
                systemParams.dhcp = std::stoi(param);
                if (!systemParams.dhcp) {
                    if (httpd_query_key_value(buf, "ip1", param, sizeof(param)) == ESP_OK) {
                        systemParams.ip[0] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ip2", param, sizeof(param)) == ESP_OK) {
                        systemParams.ip[1] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ip3", param, sizeof(param)) == ESP_OK) {
                        systemParams.ip[2] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ip4", param, sizeof(param)) == ESP_OK) {
                        systemParams.ip[3] = std::stoi(param);
                    }

                    if (httpd_query_key_value(buf, "ma1", param, sizeof(param)) == ESP_OK) {
                        systemParams.mask[0] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ma2", param, sizeof(param)) == ESP_OK) {
                        systemParams.mask[1] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ma3", param, sizeof(param)) == ESP_OK) {
                        systemParams.mask[2] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "ma4", param, sizeof(param)) == ESP_OK) {
                        systemParams.mask[3] = std::stoi(param);
                    }

                    if (httpd_query_key_value(buf, "gw1", param, sizeof(param)) == ESP_OK) {
                        systemParams.gateway[0] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "gw2", param, sizeof(param)) == ESP_OK) {
                        systemParams.gateway[1] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "gw3", param, sizeof(param)) == ESP_OK) {
                        systemParams.gateway[2] = std::stoi(param);
                    }
                    if (httpd_query_key_value(buf, "gw4", param, sizeof(param)) == ESP_OK) {
                        systemParams.gateway[3] = std::stoi(param);
                    }
                }
            }
            saveSystemParams();
        }
    }
}

std::string webBrakeJson() {
    std::string json = "{";
    json += "\"calibrate\":\"";
    json += (brakeMotorParams.calibrate == true ? "checked" : "0");

    json += "\",\"inverseEncoder\":\"";
    json += (brakeMotorParams.inverseEncoder == true ? "checked" : "0");

    json += "\",\"encoderAngle\":\"";
    json += std::to_string(brakeMotorParams.encoderAngle);

    json += "\",\"voltageLimit\":\"";
    json += std::to_string(brakeMotorParams.voltageLimit);

    json += "\"}";
    return json;
}

void webBrakeSet(httpd_req_t *req) {
    size_t buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        char*  buf;
        buf = (char*)malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[50];
            if (httpd_query_key_value(buf, "calibrate", param, sizeof(param)) == ESP_OK) {
                brakeMotorParams.calibrate = std::stoi(param);
            }
            if (httpd_query_key_value(buf, "inverseEncoder", param, sizeof(param)) == ESP_OK) {
                brakeMotorParams.inverseEncoder = std::stoi(param);
            }
            if (httpd_query_key_value(buf, "encoderAngle", param, sizeof(param)) == ESP_OK) {
                brakeMotorParams.encoderAngle = std::stof(param);
            }
            if (httpd_query_key_value(buf, "voltageLimit", param, sizeof(param)) == ESP_OK) {
                brakeMotorParams.voltageLimit = std::stof(param);
            }
            saveBrakeMotorParams();
        }
    }
}

std::string webGearboxJson() {
    std::string json = "{";
    json += "\"calibrate\":\"";
    json += (gearboxMotorParams.calibrate == true ? "checked" : "0");

    json += "\",\"inverseEncoder\":\"";
    json += (gearboxMotorParams.inverseEncoder == true ? "checked" : "0");

    json += "\",\"encoderAngle\":\"";
    json += std::to_string(gearboxMotorParams.encoderAngle);

    json += "\",\"voltageLimit\":\"";
    json += std::to_string(gearboxMotorParams.voltageLimit);

    json += "\",\"velLimitHard\":\"";
    json += std::to_string(gearboxMotorParams.velLimitHard);

    json += "\",\"velProp\":\"";
    json += std::to_string(gearboxMotorParams.velProp);

    json += "\",\"velIntegral\":\"";
    json += std::to_string(gearboxMotorParams.velIntegral);

    json += "\",\"velDiff\":\"";
    json += std::to_string(gearboxMotorParams.velDiff);

    json += "\",\"velRamp\":\"";
    json += std::to_string(gearboxMotorParams.velRamp);

    json += "\",\"velLimit\":\"";
    json += std::to_string(gearboxMotorParams.velLimit);

    json += "\",\"velFilter\":\"";
    json += std::to_string(gearboxMotorParams.velFilter);

    json += "\",\"angleProp\":\"";
    json += std::to_string(gearboxMotorParams.angleProp);

    json += "\",\"angleLimit\":\"";
    json += std::to_string(gearboxMotorParams.angleLimit);

    json += "\"}";
    return json;
}

void webGearboxSet(httpd_req_t *req) {
    size_t buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        char*  buf;
        buf = (char*)malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[50];
            if (httpd_query_key_value(buf, "calibrate", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.calibrate = std::stoi(param);
            }
            if (httpd_query_key_value(buf, "inverseEncoder", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.inverseEncoder = std::stoi(param);
            }
            if (httpd_query_key_value(buf, "encoderAngle", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.encoderAngle = std::stof(param);
            }
            if (httpd_query_key_value(buf, "voltageLimit", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.voltageLimit = std::stof(param);
            }
            if (httpd_query_key_value(buf, "velLimitHard", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.velLimitHard = std::stof(param);
            }
            if (httpd_query_key_value(buf, "velProp", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.velProp = std::stof(param);
            }
            if (httpd_query_key_value(buf, "velIntegral", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.velIntegral = std::stof(param);
            }
            if (httpd_query_key_value(buf, "velDiff", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.velDiff = std::stof(param);
            }
            if (httpd_query_key_value(buf, "velRamp", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.velRamp = std::stof(param);
            }
            if (httpd_query_key_value(buf, "velLimit", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.velLimit = std::stof(param);
            }
            if (httpd_query_key_value(buf, "velFilter", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.velFilter = std::stof(param);
            }
            if (httpd_query_key_value(buf, "angleProp", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.angleProp = std::stof(param);
            }
            if (httpd_query_key_value(buf, "angleLimit", param, sizeof(param)) == ESP_OK) {
                gearboxMotorParams.angleLimit = std::stof(param);
            }
            saveGearboxMotorParams();
        }
    }
}

std::string webStatusJson() {
    std::string json = "{";
    json += "\"brakeMotorEnable\":\"";
    json += (motorBrake.enabled ? "checked" : "0");

    json += "\",\"brakeRotation\":\"";
    json += std::to_string(motorBrake.shaft_angle);

    json += "\",\"brakeTarget\":\"";
    json += std::to_string(motorBrake.target);

    json += "\",\"gearboxMotorEnable\":\"";
    json += (motorBox.enabled ? "checked" : "0");

    json += "\",\"gearboxRotation\":\"";
    json += std::to_string(motorBox.shaft_angle);

    json += "\",\"gearboxTarget\":\"";
    json += std::to_string(motorBox.target);

    json += "\"}";
    return json;
}