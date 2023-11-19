/*
 *	===============================================================================================
 *  File Name   :   hcsr04.cpp
 *
 *  Description :   Driver for HCSR04 Ultrasonic sensor
 * 					Used to gather data and process into distance measurements
 *
 *  Contributors:   Nick Rizzo
 *	===============================================================================================
 */

#ifndef HCSR04_CPP_
#define HCSR04_CPP_

#include "hcsr04.h"
#include "main.h"
#include "cmsis_os.h"
// uS/148 = inch
// detection of within 3 inch = 3 * 148 = N uS = 444uS

//set up timer with 10khz ticks
/*
state machine


state IDLE{

next_state = TRIG_PULSE
}
state TRIG_PULSE{

next_state = ECHO_RECEIVED
}
state ECHO_RECEIVED{

next_state = WAIT
}

state WAIT{
// wait for 60ms (600 ticks)
// reset timer
next_state = TRIG_PULSE
}
*/
hcsr04::hcsr04(TIM_HandleTypeDef *timer_in, GPIO_st *trig_pin_in, GPIO_st *echo_pin_in){
    timer = timer_in;
    trig_pin = trig_pin_in;
    echo_pin = echo_pin_in;
}

void hcsr04::check_distance(){
    static uint16_t timer_val;
    switch(curr_state){
        case(IDLE):{
            next_state = TRIG_ASSERT;
            break;
        }
        case(TRIG_ASSERT):{
            // pulse assert for 10us (10 tick)
            HAL_GPIO_WritePin(trig_pin->pin_port, trig_pin->pin_num, GPIO_PIN_SET);
            HAL_TIM_Base_Start(timer);
            timer_val = __HAL_TIM_GET_COUNTER(timer);
            next_state = TRIG_DEASSERT;
            break;
        }
        case(TRIG_DEASSERT):{
               // deassert
            if (__HAL_TIM_GET_COUNTER(timer) - timer_val >= 10){
                HAL_GPIO_WritePin(trig_pin->pin_port, trig_pin->pin_num, GPIO_PIN_RESET);
                HAL_TIM_Base_Stop(timer);
                timer->Instance->CNT = 0;
                next_state = ECHO_RECEIVED;
            }
            else{
                next_state = TRIG_DEASSERT;
            }
            break;
        }
        case(ECHO_RECEIVED):{
            // start timer for 444us (440us) = 44 ticks)
            if(echo_assert){
                echo_assert = 0;
                HAL_TIM_Base_Start(timer);
                timer_val = __HAL_TIM_GET_COUNTER(timer);
                next_state = CHECK_ECHO_AFTER_TIM;
            }
            else{
                next_state = ECHO_RECEIVED;
            }
            break;
        }
        case(CHECK_ECHO_AFTER_TIM):{
            // at end of timer if echo is deasserted (<3inch) display alert
           if (__HAL_TIM_GET_COUNTER(timer) - timer_val >= 444){
                HAL_TIM_Base_Stop(timer);
                timer->Instance->CNT = 0;
                if (!HAL_GPIO_ReadPin(echo_pin->pin_port, echo_pin->pin_num)){
                    send_alert = 1;
                }
                next_state = TRIG_ASSERT;
            }
            else{
                next_state = CHECK_ECHO_AFTER_TIM;
            }
            break;
        }
    }

    curr_state = next_state;
}


#endif