/*
 *	===============================================================================================
 *  File Name   :   hcsr04.h
 *
 *  Description :   Header file for HCSR04 Ultrasonic sensor
 *
 *  Contributors:   Nick Rizzo
 *	===============================================================================================
 */
#ifndef HCSR04_H_
#define HCSR04_H_

#include "main.h"


struct GPIO_st{
    GPIO_TypeDef *pin_port;
    uint16_t pin_num;
};

enum hcsr04_state{
    IDLE,
    TRIG_ASSERT,
    TRIG_DEASSERT,
    ECHO_RECEIVED,
    CHECK_ECHO_AFTER_TIM
};

class hcsr04{
    public:
        hcsr04(TIM_HandleTypeDef *timer_in, GPIO_st *trig_pin_in, GPIO_st *echo_pin_in);
        void check_distance();
        bool send_alert = 0;
        volatile bool echo_assert = 0;

    private:
        hcsr04_state curr_state = IDLE;
        hcsr04_state next_state = IDLE;
        TIM_HandleTypeDef *timer;
        GPIO_st *trig_pin;
        GPIO_st *echo_pin;

};

#endif