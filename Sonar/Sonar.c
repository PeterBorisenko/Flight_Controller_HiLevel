#include "Sonar.h"
#include "../Macro.h"
#include "../Timer1_mega328.h"
#include <avr/io.h>

#define SONAR_TIME_CALC(x) (x)
#define SONAR_RANGE_CALC(x)	(x/(29)/2)


/*******************************************************
*
*		COMMON
*
********************************************************/
void dirTgl() {
	//BIT_read(SONAR_DIR, SONAR_PIN)?BIT_clear(SONAR_DIR, SONAR_PIN):BIT_set(SONAR_DIR, SONAR_PIN);
	BIT_tgl(SONAR_DIR, SONAR_PIN);
}
/*******************************************************
*
*		REGULAR
*
********************************************************/
void sonarInit(uint8_t mode) {
	// timer1 init
	OCR1A= SONAR_ARM_TIME;
	tmr1IntOn(OCIE1A);
	tmr1IntOn(TOIE1);
	tmr1IntOn(OCIE1A);
	tmr1Flush();
}

void sonarArm() {
	sonar_state= SONAR_STARTED;
	BIT_set(SONAR_DIR, SONAR_PIN);
	BIT_set(SONAR_PORT, SONAR_PIN);
	tmr1SetMode(TMR1_CTC_MODE);
	sonarArmTimerStart();
	tmr1Start(SONAR_TMR_PSK);
}

void sonarCaptureStart() {
	tmr1Start(SONAR_TMR_PSK); // Timer start
}

void sonarCaptureStop() {
	tmr1Stop();
	add= ICR1;
	sonar_state= SONAR_READY;
}

/*******************************************************
*
*		HI-LEVEL
*
********************************************************/

uint32_t sonarGetWidth() {
	uint32_t temp= (0xFFFF * cycleBuffer);
	return (temp + add - sub);
}

uint32_t sonarGetRange() {
	uint32_t result;
	result= SONAR_RANGE_CALC(SONAR_TIME_CALC(sonarGetWidth()));
	sonar_state= SONAR_IDLE;
	return result;
}

/*******************************************************
*
*		HANDLERS
*
********************************************************/
void sonarArmHandler() {
	BIT_clear(SONAR_PORT, SONAR_PIN);
	tmr1Stop();
	tmr1Flush();
	tmr1SetMode(TMR1_CAPTURE_MODE);
	dirTgl();
}

void sonarCaptureHandler() {
	switch (sonar_state) {
		case SONAR_RUNNING:
			sonarCaptureStop();
			break;
		case SONAR_STARTED:
			sub= ICR1;
			sonar_state= SONAR_RUNNING;
			break;
	}
}

void sonarIncreaseCycles() {
	if (cycleBuffer < SONAR_TIMEOUT)
	{
		cycleBuffer++;
	}
	else {
		sonar_state= SONAR_FAULT;
		// TODO: abort capture
	}
}
