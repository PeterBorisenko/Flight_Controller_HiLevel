#include "CPPM.h"

typedef struct {
	uint8_t state;
	uint8_t val;
} CPPM_In_t;
	
	

void CPPM_decoderHandler() { // one or more of pins 2~7 have changed state
	
}

void CPPM_init() {
	tmr0IntOn(TMR0_OVF_INT);
	tmr0Start(TMR0_PSC_1024);
}