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
#include "Compass/LSM303D.h"
#include "Proximity/Proximity.h"
#include "RC/CPPM.h"

void initOsc();
void setPowerReduction();
void prepareSystem();
void prepareCompass();
void prepareSonar();
void prepareRC();
#endif /*SYSTEM_H*/