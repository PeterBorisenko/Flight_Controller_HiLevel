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
//-------------------------------------
#define  sendDataLength 13
volatile uint8_t sendBufferIndex= 0;
volatile usartState_t usartState;

volatile static uint8_t FLAGS= 0x00;
#define INSTR_READY 0x02

// Vector instructions for drive system
typedef struct  
{
	vect_float_t required_vect;
	int8_t rotation;
} Motion_t;

typedef union {
	Motion_t motion;
	uint8_t byteToSend[sendDataLength];
} Instruction_t;

Instruction_t Instruction;
Baro_t Baro;
LSM303_t Compass;

volatile static Instruction_t * pInstruction= &Instruction;
Baro_t * pBaro= &Baro;
LSM303_t * pCompass= &Compass;

CPPM_In_t cppm[CPPM_CHANNELS];
uint8_t buffer[sendDataLength];


void startSending();
void sendMessage(volatile Instruction_t *, uint8_t *);
void NAV();
void CONT();



void getAltitude() {
	BMP085Convert(pBaro);
	BMP085CalculateAltitude(pBaro);
}

int main(void)
{
	prepareSystem();
	prepareRC();
	prepareSonar();
	prepareCompass(pCompass);
	
    while(1) {
		//NAV();
		CONT();
    }
}

void NAV() { // this task polls sensors and calculates position
	
}

void CONT() {
 // this task receives RF data and makes instructions for driver system
	do {
		uint8_t buf[CPPM_CHANNELS];
		for (uint8_t i= 0; i < CPPM_CHANNELS; i++)
		{
			if(cppm[i].state) { // Copy RC values
				buf[i]= cppm[i].val;
			}
			BIT_set(FLAGS, INSTR_READY);
		}
		if (buf[CH_T]>RC_TRESHOLD)
		{
			if(buf[CH_T]<RC_MIDDLE){
				pInstruction->motion.required_vect.Z= POSITIVE_CHANGE(buf[CH_T]);
				
			}
			else if(buf[CH_T]>RC_MIDDLE){
				pInstruction->motion.required_vect.Z= NEGATIVE_CHANGE(buf[CH_T]);
				
			}
		}
		if (buf[CH_X]>RC_TRESHOLD)
		{
			if(buf[CH_X]<RC_MIDDLE){
				pInstruction->motion.required_vect.Y= POSITIVE_CHANGE(buf[CH_X]);
			}
			else if(buf[CH_X]>RC_MIDDLE){
				pInstruction->motion.required_vect.Y= NEGATIVE_CHANGE(buf[CH_X]);
			}
		}
		if (buf[CH_Y]>RC_TRESHOLD)
		{
			if(buf[CH_Y]<RC_MIDDLE){
				pInstruction->motion.required_vect.X= POSITIVE_CHANGE(buf[CH_Y]);
			}
			else if(buf[CH_Y]>RC_MIDDLE){
				pInstruction->motion.required_vect.X= NEGATIVE_CHANGE(buf[CH_Y]);
			}
		}
		if (buf[CH_Z]>RC_TRESHOLD)
		{
			if(buf[CH_Z]<RC_MIDDLE){
				pInstruction->motion.rotation= (int8_t)buf[CH_Z];
			}
			else if(buf[CH_Z]>RC_MIDDLE){
				pInstruction->motion.rotation= (int8_t)(RC_MAX - (buf[CH_Z] - RC_MIDDLE));
			}
		}
		if (buf[CH_A]>RC_TRESHOLD)
		{
			// Altitude, course or velocity
		}
		if (buf[CH_L]>RC_TRESHOLD)
		{
			// Automatic slow landing using Sonar
		}
	} while(!BIT_read(FLAGS, INSTR_READY));
	startSending();
	BIT_clear(FLAGS, INSTR_READY);
}

 // Begin of a message sending
 // Sends first byte of header and turns ON UD interrupt
void startSending() {
	sendChar(HEADER&0xFF);
	uartIntOn(USART_UD_INT);
}

 // Sends the body of instruction message
void sendMessage(volatile Instruction_t * msg, uint8_t * buf) {
	for (uint8_t i= 0; i<= sendDataLength; i++) {
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
			case 4: case 8: case 12: case sendDataLength:
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
			case sendDataLength:								// if was sent the last byte - turn OFF UD interrupt
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
			if (sendBufferIndex != sendDataLength) {			// if received ACK and that was not after sending the last byte - send next
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
	
	uint8_t curState= (DDRD >> 2);
	uint8_t val= curState^PCINTprevState2;
	for (uint8_t i= 0; i < 6; i++)
	{
		if ((val >> i)&0x01)
		{
			CPPM_decoderHandler(&cppm[i]);
		}
	}
	PCINTprevState2=curState ;
}