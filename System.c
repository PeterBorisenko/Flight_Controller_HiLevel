/*
 * System.c
 *
 * Created: 12/7/2014 3:22:20 AM
 *  Author: Disgust
 */ 

#include "System.h"


void prepareSystem()
{
    WDTCSR|= (1 << WDE)|(1 << WDIE);
    //WDTCSR|=(0b111 << WDP0);
	initOsc();
    setPowerReduction();
}

 void setPowerReduction() {
     PRR|= (1 << PRTIM2)|(1 << PRSPI);
 }
 
 void prepareCompass(LSM303_t * compass) {
	 LSM303D_Init(compass, device_DLM, sa0_auto);
	 // LSM303D_EnableDefault(compass);
 }

 void prepareSonar() {
	 sonarInit(USE_INTERRUPT);
 }

 void prepareRF() {
	 DDRD&= ~(0xFF << 2); // Arduino mini pins 2 - 7
	 CPPM_init(PINCH_GR_2, (0xFF << 2), TMR0_PSC_1024);
 }

 void prepareBarometer() {
	 BMP085Calibration();
 }
 
 void initOsc() {
 	
}
 