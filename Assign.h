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
typedef enum {
		USART_IDLE,
		USART_HEADER,
		USART_WORK,
		USART_ASKED,
		USART_ASKS,
		SEND_STAT
	} usartState_t;

// Message parts
#define HEADER      0x1010
#define ACK         0x20
#define NACK        0x21
#define ARMED		0x30
#define FAULT		0x31
#define ASK_STATUS	0x32

typedef struct {
	int16_t X;
	int16_t Y;
	int16_t Z;
} vect_t;

typedef struct {
	int32_t X;
	int32_t Y;
	int32_t Z;
} vect_int_t;

typedef struct {
	float X;
	float Y;
	float Z;
} vect_float_t;

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

// RC defines
#define CPPM_CHANNELS 6
#define RC_TRESHOLD 16
#define RC_STEP_WIDTH 16
#define RC_MIDDLE	0x7F
#define RC_MAX	0xFF
#define CH_Z	0x0	// Yaw
#define CH_X	0x1 // Roll
#define CH_Y	0x2 // Pitch
#define CH_T	0x3 // Throttle
#define CH_A	0x4 // Autopilot (?)
#define CH_L	0x5 // Landing

#define POSITIVE_CHANGE(x) ((float)(x/0xFF))
#define NEGATIVE_CHANGE(x) (0 - ((float)(x - RC_MIDDLE)/0xFF)) // TODO: Check this

#endif /* ASSIGN_H_ */