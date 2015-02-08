#include "Sonar.h"
#include "../Macro.h"
#include "../Assign.h"
#include "../Timer1_mega328.h"
#include <avr/io.h>

#define SONAR_MULT	(SONAR_TMR_PSC/F_CPU) // Time per Tmr tick
#define SONAR_RES	(SONAR_MULT*SPEED_OF_SOUND/2) // Minimum range
#define SONAR_TIME_CALC(x) (x*SONAR_MULT)
#define SONAR_RANGE_CALC(x)	(x*SONAR_RES)


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
	tmr1SetOCRA(SONAR_ARM_TIME);
	tmr1NoiseCancelerOn();
	tmr1IntOn(OCIE1A);
	tmr1IntOn(TOIE1);
	tmr1IntOn(OCIE1A);
	tmr1Flush();
	BIT_set(SONAR_DIR, SONAR_PIN);
	BIT_clear(SONAR_PORT, SONAR_PIN);
}

void sonarArm() {
	sonar_state= SONAR_STARTED;
	BIT_set(SONAR_PORT, SONAR_PIN);
	tmr1SetMode(TMR1_CTC_MODE);
	tmr1Start(SONAR_TMR_PSC);
}

void sonarCaptureStart() {
	tmr1Start(SONAR_TMR_PSC); // Timer start
}

void sonarCaptureStop() {
	tmr1Stop();
	dirTgl();
	add= TCNT1;
	tmr1Flush();
	sonar_state= SONAR_READY;
}

/*******************************************************
*
*		HI-LEVEL
*
********************************************************/

uint16_t sonarGetWidth() {
	return (add - sub);
}

uint16_t sonarGetRange() {
	uint16_t result;
	result= SONAR_RANGE_CALC(sonarGetWidth());
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
			sub= TCNT1;
			sonar_state= SONAR_RUNNING;
			break;
	}
}

void sonarOutOfRange() {
	sonarCaptureStop();
	sonar_state= SONAR_OUT_OF_RANGE;
}