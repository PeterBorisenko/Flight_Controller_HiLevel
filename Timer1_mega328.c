
/*
 * Timer1_mega328.c
 *
 * Created: 05.02.2015 23:39:14
 *  Author: Disgust
 */ 

#include "Timer1_mega328.h"

void tmr1IntOn(uint8_t intAddr) {
	TIMSK1|= (1 << intAddr);
}

void tmr1IntOff(uint8_t intAddr) {
	TIMSK1&= ~(1 << intAddr);
}

void tmr1SetMode(uint8_t mode) {
	TCCR1B|= ((mode & 0x0E) << WGM10);
	TCCR1C|= ((mode & 0x03) << WGM12);
}

void tmr1Start(uint8_t psk) {
	TCCR1B|= (psk << CS10); // Timer start
}

void tmr1Stop() {
	TCCR1B&= ~(7 << CS10);
}

void tmr1Flush() {
	TCNT1= 0x00; // Flush counter
	TIFR1= 0x27; // Flush interrupts
}

void tmr1OutMode(uint8_t mode) {
	
}
