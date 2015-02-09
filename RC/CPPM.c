#include "CPPM.h"
	
void CPPM_init(uint8_t gr, uint8_t msk, uint8_t tmrPsc) {
	pinchOn(gr, msk);
	tmr0IntOn(TMR0_OVF_INT);
	tmr0Start(tmrPsc);
}



void CPPM_decoderHandler(CPPM_In_t * cppmIn) { // one or more of pins 2~7 have changed state
	
	if (cppmIn->state == 0x01)
	{
		cppmIn->endTim= tmr0Read();
		cppmIn->state= 0x00;
		cppmIn->val= (uint8_t)(cppmIn->endTim - cppmIn->startTim);
	} 
	else
	{
		cppmIn->startTim= tmr0Read();
		cppmIn->state= 0x01;
	}
}