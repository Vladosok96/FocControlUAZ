#include "motors.h"
#include "nvstorage.h"

spi_device_handle_t encoderBoxSpi;
spi_device_handle_t encoderBrakeSpi;

MagneticSensorSSC sensorBox = MagneticSensorSSC(&encoderBoxSpi);
MagneticSensorSSC sensorBrake = MagneticSensorSSC(&encoderBrakeSpi);

BLDCMotor motorBox = BLDCMotor(15);
BLDCMotor motorBrake = BLDCMotor(15);

BLDCDriver3PWM driverBox = BLDCDriver3PWM(5, 7, 15, 18);
BLDCDriver3PWM driverBrake = BLDCDriver3PWM(4, 6, 16, 17);


void initMotorsSpi() {
    spi_bus_config_t encoderBuscfg= {};
    encoderBuscfg.miso_io_num = 35,
    encoderBuscfg.mosi_io_num = 36;
    encoderBuscfg.sclk_io_num = 37;
    encoderBuscfg.quadwp_io_num = -1;
    encoderBuscfg.quadhd_io_num = -1;
    encoderBuscfg.max_transfer_sz = 32;  
    

    spi_device_interface_config_t encoderBox = {};
    encoderBox.command_bits = 0;
    encoderBox.address_bits = 0;
    encoderBox.dummy_bits = 0;
    encoderBox.clock_speed_hz = 1 * 1000 * 1000;
    encoderBox.spics_io_num = 38,
    encoderBox.mode = 0;
    encoderBox.queue_size = 1;

    spi_device_interface_config_t encoderBrake = {};
    encoderBrake.command_bits = 0;
    encoderBrake.address_bits = 0;
    encoderBrake.dummy_bits = 0;
    encoderBrake.clock_speed_hz = 1 * 1000 * 1000;
    encoderBrake.spics_io_num = 39,
    encoderBrake.mode = 0;
    encoderBrake.queue_size = 1;

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &encoderBuscfg, SPI_DMA_DISABLED));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &encoderBox, &encoderBoxSpi));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &encoderBrake, &encoderBrakeSpi));
}

void initMotorBrake() {
    loadBrakeMotorParams();
    sensorBrake.init();
    motorBrake.linkSensor(&sensorBrake);
    driverBrake.voltage_power_supply = 12;
    driverBrake.init();
    motorBrake.linkDriver(&driverBrake);

    motorBrake.voltage_limit = brakeMotorParams.voltageLimit;
    motorBrake.controller = MotionControlType::torque;
    motorBrake.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motorBrake.sensor_offset = -13;
    motorBrake.init();
    if (brakeMotorParams.calibrate) {
        motorBrake.initFOC();
        brakeMotorParams.encoderAngle = motorBrake.zero_electric_angle;
        brakeMotorParams.inverseEncoder = (motorBrake.sensor_direction == Direction::CCW ? true : false);
    } else {
        motorBrake.initFOC(brakeMotorParams.encoderAngle, brakeMotorParams.inverseEncoder ? Direction::CCW : Direction::CW);
    }
    motorBrake.target = 4;
    
    motorBrake.enable();
}

void initMotorBox() {
    loadGearboxMotorParams();
    sensorBox.init();
    motorBox.linkSensor(&sensorBox);
    driverBox.voltage_power_supply = 12;
    driverBox.init();
    motorBox.linkDriver(&driverBox);

    // motorBox.PID_velocity.P = gearboxMotorParams.velLimitHard;
    // motorBox.PID_velocity.I = gearboxMotorParams.velIntegral;
    // motorBox.PID_velocity.D = gearboxMotorParams.velDiff;
    // motorBox.PID_velocity.output_ramp = gearboxMotorParams.velProp;
    // motorBox.PID_velocity.limit = gearboxMotorParams.velLimit;
    // motorBox.LPF_velocity.Tf = gearboxMotorParams.velFilter;
    
    // motorBox.P_angle.P = gearboxMotorParams.angleProp;
    // motorBox.P_angle.limit = gearboxMotorParams.angleLimit;
    // motorBox.velocity_limit = gearboxMotorParams.velLimitHard;

    motorBox.sensor_offset = -13;

    motorBox.voltage_limit = gearboxMotorParams.voltageLimit;
    motorBox.controller = MotionControlType::angle;
    motorBox.init();
    if (gearboxMotorParams.calibrate) {
        motorBox.initFOC();
        gearboxMotorParams.encoderAngle = motorBox.zero_electric_angle;
        gearboxMotorParams.inverseEncoder = (motorBox.sensor_direction == Direction::CCW ? true : false);
    } else {
        motorBox.initFOC(gearboxMotorParams.encoderAngle, gearboxMotorParams.inverseEncoder ? Direction::CCW : Direction::CW);
    }
    motorBox.setPostions(1.97, 2.75, 3.05, 3.50, 0.05);
    motorBox.disable();
}