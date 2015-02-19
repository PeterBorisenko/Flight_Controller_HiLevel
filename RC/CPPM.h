/*
 * CPPM.h
 *
 * Created: 09.02.2015 0:57:46
 *  Author: Peter Borisenko, AWSMTEK.COM
 */ 


#ifndef CPPM_H_
#define CPPM_H_

#include <stdint.h>
#include "../Macro.h"
#include "../Assign.h"
#include "../PCINT_mega328.h"
#include "../Timer0_mega328.h"

typedef enum {
	LOW= 0x00,
	HIGH= 0x01
} pinState_t;

typedef struct {
	pinState_t state;
	uint8_t val;
	uint8_t startTim;
	uint8_t endTim;
} CPPM_In_t;

void CPPM_decoderHandler(CPPM_In_t *);
void CPPM_init(uint8_t, uint8_t, uint8_t);
#endif /* CPPM_H_ */
