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

volatile uint8_t sendByteCount= DATA_WIDTH;

volatile static uint8_t FLAGS= 0x00;

// Vector instructions for drive system
volatile static vect_t required_vect_X;
volatile static vect_t required_vect_Y;
volatile static vect_t required_vect_Z;

int main(void)
{
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

ISR(TIMER0_OVF_vect){

}

ISR(TIMER2_OVF_vect){

}

ISR(TIMER1_OVF_vect){ // System TIMER
	// TODO: Call Task Manager from here

}

ISR (USART_TX_vect) {

}

ISR (USART_RX_vect) {

}

ISR(WDT_vect) {
	
}