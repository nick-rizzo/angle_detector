/*
 *	===============================================================================================
 *  File Name   : mpu6050.h
 *
 *  Description : Header file for MPU6050 accelerometer/gyroscope
 *
 *  Contributors: Nick Rizzo
 *	===============================================================================================
 */
#ifndef MPU6050_H_
#define MPU6050_H_

#include "main.h"

#define GYRO_LSB_SENS 131.0
#define ACCEL_LSB_SENS 4096.0

#define SAM_RATE_DIV      0x19    // Sample rate divider = Gyro output rate / (1 + Sample rate divider)
#define GYRO_CFG          0x1B    // Gyro Configuration
#define ACCEL_CFG         0x1C    // Accelerometer Configuration
#define FIFO_EN           0x23    // Fifo Enable
#define ACCEL_XOUT_UPPER  0x3B    // Accel most recent X vals [15:8]
#define ACCEL_XOUT_LOWER  0x3C    // Accel most recent X vals [ 7:0]
#define ACCEL_YOUT_UPPER  0x3D    // Accel most recent Y vals [15:8]
#define ACCEL_YOUT_LOWER  0x3E    // Accel most recent Y vals [ 7:0]
#define ACCEL_ZOUT_UPPER  0x3F    // Accel most recent Z vals [15:8]
#define ACCEL_ZOUT_LOWER  0x40    // Accel most recent Z vals [ 7:0]
#define GYRO_XOUT_UPPER   0x43    // Gyro most recent X vals [15:8]
#define GYRO_XOUT_LOWER   0x44    // Gyro most recent X vals [ 7:0]
#define GYRO_YOUT_UPPER   0x45    // Gyro most recent Y vals [15:8]
#define GYRO_YOUT_LOWER   0x46    // Gyro most recent Y vals [ 7:0]
#define GYRO_ZOUT_UPPER   0x47    // Gyro most recent Z vals [15:8]
#define GYRO_ZOUT_LOWER   0x48    // Gyro most recent Z vals [ 7:0]
#define USER_CONTROL      0x6A    // User control register
#define POWER_MGMT_1      0x6B    // Power management 1
#define WHOAMI            0x75    //  ID reg

struct gyro_accel_st{
    volatile float x_accel;
    volatile float y_accel;
    volatile float z_accel;
    volatile float x_angle;
    volatile float y_angle;
    volatile float z_angle;
};

struct accel_reading_st{
    float x_accel;
    float y_accel;
};

class mpu6050{
    public:
        mpu6050(I2C_HandleTypeDef i2c_in, uint16_t addr_in);
        void read_gyro_data();
        accel_reading_st return_angle(gyro_accel_st data_in);
        gyro_accel_st get_gyro_data();

    private:
        void mpu6050_init();
        uint8_t read_register(uint16_t address);
        void write_register(uint8_t address, uint8_t data);
};

#endif