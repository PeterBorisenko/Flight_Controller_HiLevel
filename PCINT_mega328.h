/*
 * PCINT_mega328.h
 *
 * Created: 09.02.2015 1:00:46
 *  Author: Peter Borisenko, AWSMTEK.COM
 *------------------------------------------------*
 *  DON'T FORGET TO SWITCH ON INTERRUPTS GLOBALLY! *
 *------------------------------------------------*
 */ 


#ifndef PCINT_MEGA328_H_
#define PCINT_MEGA328_H_

#include <stdint.h>
#include <avr/io.h>
#include "Macro.h"

// pin change interrupt enable groups
#define PINCH_GR_0		0x01
#define PINCH_GR_1		0x02
#define PINCH_GR_2		0x04

volatile static uint8_t PCINTprevState0= 0, PCINTprevState1= 0, PCINTprevState2= 0;


void pinchOn(uint8_t, uint8_t);

uint8_t getPinchPin();

#endif /* PCINT_MEGA328_H_ */