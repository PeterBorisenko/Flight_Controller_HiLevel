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

#include <./math.h>	// To calculate altitude
#include "./Communication.h"

#define BMP085_R 0xEF
#define BMP085_W 0xEE
#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)

///============Function Prototypes=========/////////////////
void BMP085_Calibration(void);

///============I2C Prototypes=============//////////////////
short bmp085ReadShort(unsigned char address);
long bmp085ReadTemp(void);
long bmp085ReadPressure(void);
void bmp085Convert(long * temperature, long * pressure);

/////=========Global Variables======////////////////////
short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;

/*	
	long temperature = 0;
	long pressure = 0;
	long altitude = 0;
	double temp = 0;

	BMP085_Calibration();
	
	bmp085Convert(&temperature, &pressure);
		
	temp = (double) pressure/101325;
	temp = 1-pow(temp, 0.19029);
	altitude = round(44330*temp);
*/



#endif /* BMP085_H_ */