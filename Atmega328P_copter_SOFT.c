/*
 * Atmega328P_copter_SOFT.c
 *
 * Created: 02.02.2015 8:14:09
 *  Author: Disgust
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
/*#include <avr/delay.h>*/

#include "Macro.h"
#include "Assign.h"
#include "System.h"
#include "Communication.h"

volatile uint8_t transmitByteCount= DATA_WIDTH;

volatile static uint8_t FLAGS= 0x00;

// Vector instructions for drive system
typedef struct {
	vect_t required_vect_X;
	vect_t required_vect_Y;
	vect_t required_vect_Z;
} Required_t;

Required_t Required;
Baro_t Baro;
LSM303_t Compass;

volatile static Required_t * pRequired= &Required;
Baro_t * pBaro= &Baro;
LSM303_t * pCompass= &Compass;

CPPM_In_t * cppmIn0;
CPPM_In_t * cppmIn1;
CPPM_In_t * cppmIn2;
CPPM_In_t * cppmIn3;
CPPM_In_t * cppmIn4;
CPPM_In_t * cppmIn5;

CPPM_In_t cppmPins[]= {cppmIn0, cppmIn1, cppmIn2, cppmIn3, cppmIn4, cppmIn5};

void getAltitude() {
	BMP085Convert(pBaro);
	BMP085CalculateAltitude(pBaro);
}

void main(void)
{
	prepareSystem();
	prepareRF();
	prepareSonar();
	prepareCompass(pCompass);
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

ISR(TIMER0_OVF_vect){ 

}

ISR(TIMER2_OVF_vect){ // System TIMER
	
	// TODO: Call Task Manager from here
}

ISR (USART_TX_vect) {

}

ISR (USART_RX_vect) {

}

ISR(WDT_vect) {
	
}

ISR(TIMER1_OVF_vect) {
	sonarOutOfRange();
}

ISR(TIMER1_CAPT_vect) {
	sonarCaptureHandler();
}

ISR(TIMER1_COMPA_vect) {
	sonarArmHandler();
}

ISR(PCINT2_vect) {
	// TODO: Make selection by interrupt source pin
	
	static uint8_t prevState;
	static uint8_t curState= (DDRD >> 2);
	uint8_t val= curState^prevState;
	for (uint8_t i= 0; i < 6; i++)
	{
		if ((val >> i)&0x01)
		{
			CPPM_decoderHandler(cppmPins[i]);
		}
	}

}