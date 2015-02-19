#include "PCINT_mega328.h"

void pinchOn(uint8_t grNum, uint8_t msk)
{
	PCICR= grNum;
	switch(grNum) {
		case 0x01:default:
		PCMSK0= msk;
		break;
		case 0x02:
		PCMSK1= msk;
		break;
		case 0x03:
		PCMSK0= msk;
		PCMSK1= msk;
		break;
		case 0x04:
		PCMSK2= msk;
		break;
		case 0x05:
		PCMSK0= msk;
		PCMSK2= msk;
		break;
		case 0x06:
		PCMSK2= msk;
		PCMSK1= msk;
		break;
		case 0x07:
		PCMSK0= msk;
		PCMSK1= msk;
		PCMSK2= msk;
		break;
	}
}



