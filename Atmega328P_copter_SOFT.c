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

typedef void (*task)(void);
task taskQueue[255];

volatile const uint8_t sendDataLength= 13;
volatile uint8_t sendBufferIndex= 0;
volatile usartState_t usartState;

volatile static uint8_t FLAGS= 0x00;

// Vector instructions for drive system
typedef struct  
{
	vect_float_t required_vect;
	int8_t rotation;
} Motion_t;

typedef union {
	Motion_t motion;
	uint8_t byteToSend[13];
} Instruction_t;

Instruction_t Instruction;
Baro_t Baro;
LSM303_t Compass;

volatile static Instruction_t * pInstruction= &Instruction;
Baro_t * pBaro= &Baro;
LSM303_t * pCompass= &Compass;

CPPM_In_t cppm[CPPM_CHANNELS];

uint8_t buffer[13];

void getAltitude() {
	BMP085Convert(pBaro);
	BMP085CalculateAltitude(pBaro);
}

void main(void)
{
	prepareSystem();
	prepareRC();
	prepareSonar();
	prepareCompass(pCompass);
	
    while(1) {
        //TODO:: Please write your application code 
    }
}

void NAV() { // this task polls sensors and calculates position
	
}

void CONT() { // this task receives RF data and makes instructions for driver system
	while(1) {
		for (uint8_t i= 0; i < CPPM_CHANNELS; i++)
		{
			if (cppm[i].state)
			{
				
			} 
		}
	}
}

void IDLE() { // this is an empty task
	while (1) {
		;;
	}
}

void TASK_(void * task) { // task container
	
}

 // Begin of a message sending
 // Sends first byte of header and turns ON UD interrupt
void startSending() {
	sendChar(HEADER&0xFF);
	uartIntOn(USART_UD_INT);
}

 // Sends the body of instruction message
void sendMessage(Instruction_t * msg, uint8_t * buf) {
	for (uint8_t i= 0; i<= 13; i++) {
		buf[i]= msg->byteToSend[i];
	}
	sendBufferIndex++;
	sendChar(buf[0]);
	uartIntOn(USART_UD_INT);
}

ISR(TIMER0_OVF_vect){ 
	
}

ISR(TIMER2_OVF_vect){ // System TIMER
	
	// TODO: Call Task Manager from here
}

ISR (USART_TX_vect) {
	// if TX completed - wait for answer
	if (usartState == USART_WORK) {
		switch(sendBufferIndex) {
			case 4: case 8: case 12: case 13:
				// TODO: start ACK receive timer
				break;
			default:
				break;
		}
	}
}

ISR (USART_UDRE_vect) {
	if (usartState == USART_WORK) {				
		switch(sendBufferIndex){				// previously sent byte
			case 13:								// if was sent the last byte - turn OFF UD interrupt
				sendBufferIndex= 0;
				uartIntOff(USART_UD_INT);
				break;
			case 3: case 7: case 11: case 12:		// if next byte is the last byte of vector type element - send next and turn OFF UD interrupt
				sendChar(buffer[sendBufferIndex]);
				uartIntOff(USART_UD_INT);
				break;
			default:								// just send the next byte from the buffer
				sendChar(buffer[sendBufferIndex]);
				sendBufferIndex++;					// increment counter after sending !!!
				break;
		}
		return;
	}
	else if (usartState == USART_IDLE) { // Sends the second byte of message header
		sendChar((HEADER >> 8)&0xFF);
		uartIntOff(USART_UD_INT);
	}
}

ISR (USART_RX_vect) {
	// if received ACK - send next message if exists or change state
	// if received NACK - send message from beginning
	uint8_t resp= receiveChar();
	if (usartState == USART_WORK) {
		if (resp == ACK) {				
			if (sendBufferIndex != 13) {			// if received ACK and that was not after sending the last byte - send next
				sendBufferIndex++;
				sendChar(buffer[sendBufferIndex]);
				uartIntOn(USART_UD_INT);
			} 
			else {									// if received ACK and that was after sending the last byte - change state and exit				
				sendBufferIndex= 0;
				usartState= USART_IDLE;
				return;
			}
		} 
		else if (resp == NACK) {
			// TODO: error processing
			sendBufferIndex= 0;
			sendChar(buffer[sendBufferIndex]);
			uartIntOn(USART_UD_INT);
			return;
		}
	}
	else if (usartState == USART_IDLE) {
		if (resp == ACK) {
			usartState= USART_WORK;
			sendMessage(pInstruction, buffer);
		} 
		else if (resp == NACK) {
			// TODO: error processing
			startSending();
		}
	}
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
	
	uint8_t prevState;
	uint8_t curState= (DDRD >> 2);
	uint8_t val= curState^prevState;
	for (uint8_t i= 0; i < 6; i++)
	{
		if ((val >> i)&0x01)
		{
			CPPM_decoderHandler(&cppm[i]);
		}
	}

}