/*
 * IncFile1.h
 *
 * Created: 09.02.2015 21:51:44
 *  Author: Disgust
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include <avr/io.h>
#include "Macro.h"

#define TMR0_NO_PSC		0x01
#define TMR0_PSC_8		0x02
#define TMR0_PSC_64		0x03
#define TMR0_PSC_256	0x04
#define TMR0_PSC_1024	0x05
#define TMR0_EXT_FAL	0x06
#define TMR0_EXT_RIS	0x07

#define TMR0_OVF_INT	0x01
#define TMR0_OCA_INT	0x02
#define TMR0_OCB_INT	0x04

void tmr0IntOn(uint8_t);

void tmr0IntOff(uint8_t);

void tmr0SetMode(uint8_t);

void tmr0Start(uint8_t);

void tmr0Stop();

void tmr0Flush();

void tmr0OutMode(uint8_t);

void tmr0SetOCRA(uint8_t);

void tmr0SetOCRB(uint8_t);

uint8_t tmr0Read();


#endif /* INCFILE1_H_ */