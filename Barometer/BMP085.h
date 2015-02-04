/*
 * BMP085.h
 *
 * Created: 03.02.2015 1:05:21
 *  Author: Disgust
 */ 


#ifndef BMP085_H_
#define BMP085_H_

/*
    
	Redesigned by: Disgust
	02.02.2015
	from 
	BMP085 Test Code
	by: Jim Lindblom
	
	Test code for the BMP085 Barometric Pressure Sensor.
	We'll first read all the calibration values from the sensor.
	Then the pressure and temperature readings will be read and calculated.
	Also attempts to calculate altitude (remove comments)
	The sensor is run in ultra low power mode.
	Tested on a 3.3V 8MHz Arduino Pro
	A4 (PC4) -> SDA
	A5 (PC5) -> SCL
	No Connection to EOC or XCLR pins
*/

#include <math.h>	// To calculate altitude
#include <stdint.h>
#include "../Communication.h"

#define BMP085_R 0xEF
#define BMP085_W 0xEE
#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)

// Barometric data struct
typedef struct {
	int64_t	temperature;
	int64_t	pressure;
	int64_t	altitude;
} Baro_t;

///============Function Prototypes=========/////////////////
void BMP085Calibration(void);

///============I2C Prototypes=============//////////////////
int16_t BMP085ReadShort(uint8_t);
int64_t BMP085ReadTemp(void);
int64_t BMP085ReadPressure(void);
void BMP085Convert(Baro_t *);
void BMP085CalculateAltitude(Baro_t *);

/////=========Global Variables======////////////////////
int16_t ac1;
int16_t ac2; 
int16_t ac3; 
uint16_t ac4;
uint16_t ac5;
uint16_t ac6;
int16_t b1; 
int16_t b2;
int16_t mb;
int16_t mc;
int16_t md;


#endif /* BMP085_H_ */