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

volatile static Required_t * Required;
Baro_t * Baro;

void getAltitude() {
	BMP085Convert(Baro);
	BMP085CalculateAltitude(Baro);
}

void main(void)
{
	prepareSystem();
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

ISR(TIMER0_OVF_vect){ // System TIMER
	// TODO: Call Task Manager from here

}

ISR(TIMER2_OVF_vect){

}

ISR (USART_TX_vect) {

}

ISR (USART_RX_vect) {

}

ISR(WDT_vect) {
	
}

ISR(TIMER1_OVF_vect) {
	sonarIncreaseCycles();
}

ISR(TIMER1_CAPT_vect) {
	sonarCaptureHandler();
}

ISR(TIMER1_COMPA_vect) {
	sonarArmHandler();
}

ISR(PCINT0_vect) {
	// TODO: Software USART RX
}