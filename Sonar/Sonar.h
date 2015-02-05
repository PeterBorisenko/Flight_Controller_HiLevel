/*
 * Sonar.h
 *
 * Created: 05.02.2015 18:00:33
 *  Author: Disgust
 * 
 * Needs Timer to be prepared in Capture mode
 * with enabled OVF and other interrupts
 * TODO: Complete reference
 */ 


#ifndef SONAR_H_
#define SONAR_H_

#include <stdint.h>

// Sonar Port
#ifndef SONAR_PIN
#define SONAR_DIR DDRB
#define SONAR_PORT PORTB
#define SONAR_PIN PINB0
#endif

// Sonar States
volatile static uint8_t sonar_state;
#define SONAR_IDLE			0x00
#define	SONAR_READY			0x01
#define SONAR_RUNNING		0x02
#define SONAR_STARTED		0x03
#define SONAR_FAULT			0xFF

#define DONT_USE_INTERRUPTS	0x00
#define USE_INTERRUPT		0x01
#define SONAR_TIMEOUT		0xFF // 16-bit cycles

#define SONAR_TMR_PSK		0x05
#define SONAR_ARM_TIME		0x0F

// Variables
volatile static uint8_t cycleBuffer;
volatile static uint16_t sub;
volatile static uint16_t add;

// Prototypes
void sonarInit(uint8_t);
void sonarArm();
void dirTgl();
void sonarCaptureStart();
void sonarCaptureStop();
uint32_t sonarGetWidth();
uint32_t sonarGetRange();
void sonarIncreaseCycles();
void sonarArmHandler();
void sonarCaptureHandler();


#endif /* SONAR_H_ */