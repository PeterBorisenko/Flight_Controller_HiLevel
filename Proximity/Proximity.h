/*
 * Proximity.h
 *
 * Created: 4/19/2014 12:24:07 AM
 *  Author: Disgust
 */ 


#ifndef PROXIMITY_H_
#define PROXIMITY_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../ADC_mega328.h"
#include "../Assign.h"

typedef enum {
		ON,
		OFF
	} proxState_t;

typedef struct {
	uint8_t channel;
	proxState_t state; // ON/OFF
	adc_t rawData;
	} ProximitySensor_t;

void prepareProximitySensors();
void checkProximitySensor(ProximitySensor_t *); // search for connected IR-sensors, returns array of struct
uint8_t conversion( uint8_t ); /*ADC 8-bit, Vref 3.3V */
uint8_t linearApprox( uint8_t );
void proximityReadHandler(/* Use global var */);
void readAllProximitySensors();
#endif /* PROXIMITY_H_ */