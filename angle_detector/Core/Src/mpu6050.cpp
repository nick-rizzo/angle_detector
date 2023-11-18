#ifndef MPU6050
#define MPU6050
#include "mpu6050.h"
#include <stdio.h>
#include <math.h>

gyro_accel_st gyro_data;
uint16_t addr;
I2C_HandleTypeDef i2c;

mpu6050::mpu6050(I2C_HandleTypeDef i2c_in, uint16_t addr_in){
    i2c = i2c_in;
    addr = addr_in;
    mpu6050_init();
}

uint8_t mpu6050::read_register(uint16_t address){
    uint8_t rd_data = 0;
    HAL_I2C_Mem_Read(&i2c, addr, address, I2C_MEMADD_SIZE_8BIT, &rd_data, 1, HAL_MAX_DELAY);
    return rd_data;
}

void mpu6050::write_register(uint8_t address, uint8_t data){
    HAL_I2C_Mem_Write(&i2c, addr, address, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
}

void mpu6050::mpu6050_init(){
    uint8_t ok = read_register(WHOAMI);
    char *starting = "Initializing MPU6050 ";
    char buf[4];
    sprintf(buf, "%x\n", ok);
    HAL_UART_Transmit(&huart2, (uint8_t *)starting, 22, 10000);
    HAL_UART_Transmit(&huart2, (uint8_t *)buf, 3, 10000);

    write_register(POWER_MGMT_1, 0x00);     // enable
    write_register(SAM_RATE_DIV, 0x07);     // 1 kHz sample rate
    write_register(GYRO_CFG, 0x00);         // Range +/- 250 deg/sec
    write_register(ACCEL_CFG, 0x10);        // Range +/- 8g
}

gyro_accel_st mpu6050::get_gyro_data(){
    return gyro_data;
}

void mpu6050::read_gyro_data(){
    const volatile int16_t x_angle = (read_register(GYRO_XOUT_UPPER)<<8) | read_register(GYRO_XOUT_LOWER);
    const volatile int16_t y_angle = (read_register(GYRO_YOUT_UPPER)<<8) | read_register(GYRO_YOUT_LOWER);
    const volatile int16_t z_angle = (read_register(GYRO_ZOUT_UPPER)<<8) | read_register(GYRO_ZOUT_LOWER);
    const volatile int16_t x_accel = (read_register(ACCEL_XOUT_UPPER)<<8) | read_register(ACCEL_XOUT_LOWER);
    const volatile int16_t y_accel = (read_register(ACCEL_YOUT_UPPER)<<8) | read_register(ACCEL_YOUT_LOWER);
    const volatile int16_t z_accel = (read_register(ACCEL_ZOUT_UPPER)<<8) | read_register(ACCEL_ZOUT_LOWER);

    gyro_data.x_angle = (x_angle / GYRO_LSB_SENS);
    gyro_data.y_angle = (y_angle / GYRO_LSB_SENS);
    gyro_data.z_angle = (z_angle / GYRO_LSB_SENS);

    gyro_data.x_accel = (x_accel / ACCEL_LSB_SENS);
    gyro_data.y_accel = (y_accel / ACCEL_LSB_SENS);
    gyro_data.z_accel = (z_accel / ACCEL_LSB_SENS);
}

accel_reading_st mpu6050::return_angle(gyro_accel_st data_in){
    accel_reading_st pitch_roll_angles;
    //x roll = arctan(Yaccel / sqrt(Xaccel^2 + Zaccel^2))
    //y pitch = arctan(-Xaccel / sqrt(Yaccel^2 + Zaccel^2))
    float x_raw  = (float) data_in.x_accel;
    float y_raw  = (float) data_in.y_accel;
    float z_raw  = (float) data_in.z_accel;

    float x_angle_rad = atan(y_raw/(sqrt((x_raw*x_raw)+(z_raw * z_raw))));
    float y_angle_rad = atan((-1 * x_raw)/(sqrt((y_raw*y_raw)+(z_raw * z_raw))));

    float x_angle_deg = x_angle_rad / (3.14145/180);
    float y_angle_deg = y_angle_rad / (3.14145/180);

    pitch_roll_angles.x_accel = x_angle_deg;
    pitch_roll_angles.y_accel = y_angle_deg;

    return pitch_roll_angles;
}

#endif
