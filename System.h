/*
 * System.h
 *
 * Created: 12/7/2014 3:19:32 AM
 *  Author: Disgust
 */ 
#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include "Assign.h"

// Devices
#include "Barometer/BMP085.h"
#include "Sonar/Sonar.h"
//#include "Compass/LSM303D.h"
#include "GPS/GPS_EM_506.h"
#include "Proximity.h"
//#include "RC/"

void initOsc();
void setPowerReduction();
void prepareTimer(uint8_t, uint8_t, uint8_t);
void prepareSystem();
void prepareCompass();
void prepareSonar();
void prepareGPS();
void prepareRF();
#endif /*SYSTEM_H*/