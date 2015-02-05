#pragma once

#include <stdint.h>
#include <avr/io.h>

#define TMR1_CTC_MODE		0x04 // WGM1H = 0b01
#define TMR1_CAPTURE_MODE	0x05 // WGM1H = 0b01, WGM1L = 0b01

// Prototypes

void tmr1IntOn(uint8_t);

void tmr1IntOff(uint8_t);

void tmr1SetMode(uint8_t);

void tmr1Start(uint8_t);

void tmr1Stop();

void tmr1Flush();

void tmr1OutMode(uint8_t);