/*
 * Proximity.c
 *
 * Created: 4/19/2014 12:26:02 AM
 *  Author: Disgust
 */ 
#include "Proximity.h"

volatile ProximitySensor_t * currentMeasuringSensor;
uint8_t block= 0; // This is need for preventing multiply sensor checkings or measurings at time

ProximitySensor_t * sens[numProximityChannels];

uint8_t conversion( uint8_t _ADC ) /*ADC 8-bit, Vref 3.3V */ // TODO: change for 5V
{
	uint8_t Half_DIV;
    uint16_t Result;
    
    if(_ADC<15) return 255;		// Очень далеко. Или ошибка датчика.
    
    Half_DIV = _ADC-8;
    Half_DIV >>=1;			// Делим пополам сдвигом.
    
    Result = 1000+(uint16_t)Half_DIV;
    
    return (uint8_t)(Result/(_ADC-8));
}

uint8_t linearApprox(uint8_t input)
{
    // Too far
    if (input<21) return 255;
    
    // Line1 X=63+255-4Y
    if(input<56)
    {
        input <<=2;         		//4*Y
        return (63+(255-input));
    }
    
    // Line2 X=150-Y
    if (input<118)
    {
        return (150-input);
    }
    
    // Line3 X=63-0.25Y
    if(input<250)
    {
        input >>=2;         		//0.25*Y
        return (63-input);
    }
    
    // Too close
    return 0;
}

void prepareProximitySensors()
{
	// TODO: init adc
}

void readAllProximitySensors() {
	for (uint8_t ch; ch < numProximityChannels; ch++)
	{
		if ((!block)&&(!sens[ch]->rawData.state)) {
			checkProximitySensor(sens[ch]);
		}
	}
}

void checkProximitySensor(ProximitySensor_t * sensor)
{
	currentMeasuringSensor= sensor;
	block= 1;
	adcSelectChannel(currentMeasuringSensor->channel);
	adcStart(&currentMeasuringSensor->rawData);
	// TODO: check if adc input are not equal zero. if equal - sensor not exists
}

void proximityReadHandler() {
	adcGetData(&currentMeasuringSensor->rawData);
	if (currentMeasuringSensor->rawData.value < 10)
	{
		currentMeasuringSensor->state= OFF;
	}
	else {
		currentMeasuringSensor->state= ON;
	}
	block= 0;
}