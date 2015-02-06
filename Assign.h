/*
 * Assign.h
 *
 * Created: 11/26/2014 1:10:41 AM
 *  Author: Disgust
 */ 


#ifndef ASSIGN_H_
#define ASSIGN_H_

#include <avr/io.h>

#define F_CPU 16000000UL
#define BAUD 1200

// USART States
#define USART_IDLE  0x00
#define USART_REQ   0x01
#define HEADER_OK   0x02
#define RECEIVE_X   0x03
#define RECEIVE_Y   0x04
#define RECEIVE_Z   0x05
#define RECEIVE_ROT	0x06
#define RECEIVE_RSP	0x07
#define USART_ASKED	0x10
#define USART_ASKS	0x11
#define SEND_STAT	0x12

// Message parts
#define HEADER      0x1010
#define ACK         0x20
#define NACK        0x21
#define ARMED		0x30
#define FAULT		0x31
#define ASK_STATUS	0x32

typedef int16_t vect_t;
#define DATA_WIDTH 2

// Sonar Pins/Ports
#define SONAR_DIR		DDRB
#define SONAR_PORT		PORTB
#define SONAR_PIN		PINB0 // Arduino mini pin 8

// Gear switch Pins/Ports
#define GEAR_DIR
#define GEAR_PORT
#define GEAR_PIN		// Arduino mini pin 9

// Proximity Pins/Ports
#define F_PROX_DIR
#define F_PROX_PORT
#define F_PROX_PIN			// ADC7
#define F_PROX_ADC_MUX
#define B_PROX_DIR
#define B_PROX_PORT
#define B_PROX_PIN			// ADC6
#define B_PROX_ADC_MUX
#define L_PROX_DIR
#define L_PROX_PORT
#define L_PROX_PIN			// ADC0
#define L_PROX_ADC_MUX
#define R_PROX_DIR
#define R_PROX_PORT
#define R_PROX_PIN			// ADC1
#define R_PROX_ADC_MUX
#define U_PROX_DIR
#define U_PROX_PORT
#define U_PROX_PIN			// ADC2
#define U_PROX_ADC_MUX
#define D_PROX_DIR
#define D_PROX_PORT
#define D_PROX_PIN			// ADC3
#define D_PROX_ADC_MUX

#endif /* ASSIGN_H_ */