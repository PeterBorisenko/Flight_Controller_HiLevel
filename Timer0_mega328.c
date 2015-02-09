#include "Timer0_mega328.h"



void tmr0Start(uint8_t psc)
{
	TCCR0B|= (psc << CS00);
}

void tmr0Stop()
{
	TCCR0B&= 0x07;
}

void tmr0IntOn(uint8_t intNumber)
{
	TIMSK0|= intNumber;
}

void tmr0IntOff(uint8_t intNumber)
{
	TIMSK0&= ~intNumber;
}

void tmr0Flush()
{
	TCNT0= 0x00;
}

void tmr0SetMode(uint8_t mode)
{
	TCCR0A&= ~(0x03);
	TCCR0B&= ~(0x08);
	TCCR0A|= ((mode&0x03) << WGM00);
	TCCR0B|= ((mode >> 2) << WGM02);
}

