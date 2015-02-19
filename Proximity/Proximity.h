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
	} State_t;

typedef struct {
	State_t state; // ON/OFF
	uint8_t rawData;
	} ProximitySensor_t;

void prepareProximitySensors();
ProximitySensor_t * checkProximitySensors(); // search for connected IR-sensors, returns array of struct
void readProximitySensors(ProximitySensor_t *); // read every existed channel and store to struct
uint8_t conversion( uint8_t ); /*ADC 8-bit, Vref 3.3V */
uint8_t linearApprox( uint8_t );

#endif /* PROXIMITY_H_ */