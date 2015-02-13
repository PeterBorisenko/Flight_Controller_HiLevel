#pragma once

#include <stdint.h>
#include <avr/io.h>

#define TMR1_CTC_MODE		0x04 // WGM1H = 0b01
#define TMR1_CAPTURE_MODE	0x05 // WGM1H = 0b01, WGM1L = 0b01

// interrupt enable reg values. It can be switched on in one call:
// tmr0IntOn(TMR1_OVF_INT+TMR1_OCA_INT);
#define TMR1_OVF_INT	0x01
#define TMR1_OCA_INT	0x02
#define TMR1_OCB_INT	0x04
#define TMR1_IC_INT		0x20

// Prototypes

void tmr1IntOn(uint8_t);

void tmr1IntOff(uint8_t);

void tmr1SetMode(uint8_t);

void tmr1Start(uint8_t);

void tmr1Stop();

void tmr1Flush();

void tmr1OutMode(uint8_t);

void tmr1SetOCRA(uint8_t);

void tmr1SetOCRB(uint8_t);

void tmr1NoiseCancelerOn();
void tmr1NoiseCancelerOff();