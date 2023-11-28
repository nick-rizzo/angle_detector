# Angle Detector and Distance Sensor
## For Real Time Off-Road monitoring

### Description

This project was created in order to provide some more statistics when taking my car off road. The current statistics provided include pitch and roll angles of the vehicle and an alert system for obstacles underneath the vehicle and may pose risk of colliding with the underside.

This repo is made to work with STM32F446RE MCU using C++.

### Hardware Used:
- x1 MPU6050 accelerometer/gyroscope for angle measurement
- x1 HCSR04 ultrasonic sensor for distance measurement
- x3 SSD1306 128x64 OLED displays for information displays

### Software Used
- STM32CubeIDE
- FreeRTOS
- STM32F4 HAL as a submodule in this repo

### Planned Features
- Add the ability to use multiple distance sensors






