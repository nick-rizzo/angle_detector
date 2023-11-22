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
hcsr04::hcsr04(){

}

#endif
