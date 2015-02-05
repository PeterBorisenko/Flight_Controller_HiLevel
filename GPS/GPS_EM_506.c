/*
 * GPS.c
 *
 * Created: 04.02.2015 0:07:57
 *  Author: Disgust
 */ 

#include "GPS_EM_506.h"
#include "../Communication.h"

void GPSinit() {
}

uint8_t allBitsXor(uint8_t dat) {
	uint8_t temp= (dat & 0x01);
	for (uint8_t i= 1; i <= 7; i++)
	{
		temp^= ((dat >> i) & 0x01);
	}
	return (temp & 0x01);
}

// GPS Write commands

void GPSsetSerialPort(GPSsetSerialPort_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
				(allBitsXor(GPSbaud[gpsBaudLength - 1]) << 3)|(allBitsXor(buf->dataBits) << 2)|(allBitsXor(buf->stopBits) << 1)|(allBitsXor(buf->parity));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	SSsendChar('$');
	SSsendChar('P');
	SSsendChar('S');
	SSsendChar('R');
	SSsendChar('F');
	SSsendChar('1');
	SSsendChar('0');
	SSsendChar('0');
	//---- body
	SSsendChar(buf->proto);
	for (uint8_t i= 0; i < gpsBaudLength; i++)
	{
		SSsendChar(GPSbaud[i]);
	}
	SSsendChar(buf->dataBits);
	SSsendChar(buf->stopBits);
	SSsendChar(buf->parity);
	//---- tail
	SSsendChar('*');
	SSsendChar(ch1);
	SSsendChar(ch2);
	SSsendChar(0x0D);
	SSsendChar(0x0A);
}

void GPSinitNav(GPSinitNav_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor((uint8_t)buf->weekNo >> 8) << 3)|(allBitsXor((uint8_t)buf->weekNo) << 2)|(allBitsXor(buf->chnlCount) << 1)|(allBitsXor(buf->resetCfg));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	SSsendChar('$');
	SSsendChar('P');
	SSsendChar('S');
	SSsendChar('R');
	SSsendChar('F');
	SSsendChar('1');
	SSsendChar('0');
	SSsendChar('1');
	//---- body
	SSsendChar((uint8_t)buf->posX >> 24); // TODO: check this!
	SSsendChar((uint8_t)buf->posX >> 16);
	SSsendChar((uint8_t)buf->posX >> 8); 
	SSsendChar((uint8_t)buf->posX);
	SSsendChar((uint8_t)buf->posY >> 24);
	SSsendChar((uint8_t)buf->posY >> 16);
	SSsendChar((uint8_t)buf->posY >> 8);
	SSsendChar((uint8_t)buf->posY);
	SSsendChar((uint8_t)buf->posZ >> 24);
	SSsendChar((uint8_t)buf->posZ >> 16);
	SSsendChar((uint8_t)buf->posZ >> 8);
	SSsendChar((uint8_t)buf->posZ);
	SSsendChar((uint8_t)buf->clkOffset >> 24);
	SSsendChar((uint8_t)buf->clkOffset >> 16);
	SSsendChar((uint8_t)buf->clkOffset >> 8);
	SSsendChar((uint8_t)buf->clkOffset);
	SSsendChar((uint8_t)buf->timeOfWeek >> 24);
	SSsendChar((uint8_t)buf->timeOfWeek >> 16);
	SSsendChar((uint8_t)buf->timeOfWeek >> 8);
	SSsendChar((uint8_t)buf->timeOfWeek);
	SSsendChar((uint8_t)buf->weekNo >> 8);
	SSsendChar((uint8_t)buf->weekNo);
	SSsendChar(buf->chnlCount);
	SSsendChar(buf->resetCfg);
	//---- tail
	SSsendChar('*');
	SSsendChar(ch1);
	SSsendChar(ch2);
	SSsendChar(0x0D);
	SSsendChar(0x0A);
}

void GPSsetDGPS(GPSsetDGPS_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor('2') << 3)|(allBitsXor(buf->dataBits) << 2)|(allBitsXor(buf->stopBits) << 1)|(allBitsXor(buf->parity));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	SSsendChar('$');
	SSsendChar('P');
	SSsendChar('S');
	SSsendChar('R');
	SSsendChar('F');
	SSsendChar('1');
	SSsendChar('0');
	SSsendChar('2');
	//---- body
	SSsendChar(buf->dataBits);
	SSsendChar(buf->stopBits);
	SSsendChar(buf->parity);
	//---- tail
	SSsendChar('*');
	SSsendChar(ch1);
	SSsendChar(ch2);
	SSsendChar(0x0D);
	SSsendChar(0x0A);
}

void GPSsetQueryRateCtl(GPSsetQueryRateCtl_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor(buf->msg) << 3)|(allBitsXor(buf->mode) << 2)|(allBitsXor(buf->rate) << 1)|(allBitsXor(buf->checkSumEn));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	SSsendChar('$');
	SSsendChar('P');
	SSsendChar('S');
	SSsendChar('R');
	SSsendChar('F');
	SSsendChar('1');
	SSsendChar('0');
	SSsendChar('3');
	//---- body
	SSsendChar(buf->msg);
	SSsendChar(buf->mode);
	SSsendChar(buf->rate);
	SSsendChar(buf->checkSumEn);
	//---- tail
	SSsendChar('*');
	SSsendChar(ch1);
	SSsendChar(ch2);
	SSsendChar(0x0D);
	SSsendChar(0x0A);
}

void GPSllaNavInit(GPSinitLlaNav_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
				(allBitsXor((uint8_t)buf->weekNo >> 8) << 3)|(allBitsXor((uint8_t)buf->weekNo) << 2)|(allBitsXor(buf->channelCount) << 1)|(allBitsXor(buf->resetCfg));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	SSsendChar('$');
	SSsendChar('P');
	SSsendChar('S');
	SSsendChar('R');
	SSsendChar('F');
	SSsendChar('1');
	SSsendChar('0');
	SSsendChar('4');
	//---- body
	SSsendChar((uint8_t)buf->lat >> 24); // TODO: Check this!!!
	SSsendChar((uint8_t)buf->lat >> 16);
	SSsendChar((uint8_t)buf->lat >> 8);
	SSsendChar((uint8_t)buf->lat);
	SSsendChar((uint8_t)buf->lon >> 24);
	SSsendChar((uint8_t)buf->lon >> 16);
	SSsendChar((uint8_t)buf->lon >> 8);
	SSsendChar((uint8_t)buf->lon);
	SSsendChar((uint8_t)buf->alt >> 24);
	SSsendChar((uint8_t)buf->alt >> 16);
	SSsendChar((uint8_t)buf->alt >> 8);
	SSsendChar((uint8_t)buf->alt);
	SSsendChar((uint8_t)buf->clkOffset >> 24);
	SSsendChar((uint8_t)buf->clkOffset >> 16);
	SSsendChar((uint8_t)buf->clkOffset >> 8);
	SSsendChar((uint8_t)buf->clkOffset);
	SSsendChar((uint8_t)buf->timeOfWeek >> 24);
	SSsendChar((uint8_t)buf->timeOfWeek >> 16);
	SSsendChar((uint8_t)buf->timeOfWeek >> 8);
	SSsendChar((uint8_t)buf->timeOfWeek);
	SSsendChar((uint8_t)buf->weekNo >> 8);
	SSsendChar((uint8_t)buf->weekNo);
	SSsendChar(buf->channelCount);
	SSsendChar(buf->resetCfg);
	//---- tail
	SSsendChar('*');
	SSsendChar(ch1);
	SSsendChar(ch2);
	SSsendChar(0x0D);
	SSsendChar(0x0A);
}

void GPStglDevData(GPStglDevData_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor('1') << 3)|(allBitsXor('0') << 2)|(allBitsXor('5') << 1)|(allBitsXor(buf->debug));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	SSsendChar('$');
	SSsendChar('P');
	SSsendChar('S');
	SSsendChar('R');
	SSsendChar('F');
	SSsendChar('1');
	SSsendChar('0');
	SSsendChar('5');
	//---- body
	SSsendChar(buf->debug);
	//---- tail
	SSsendChar('*');
	SSsendChar(ch1);
	SSsendChar(ch2);
	SSsendChar(0x0D);
	SSsendChar(0x0A);
}

void GPSselDatum(GPSselDatum_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor('1') << 3)|(allBitsXor('0') << 2)|(allBitsXor('6') << 1)|(allBitsXor(buf->datum));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	SSsendChar('$');
	SSsendChar('P');
	SSsendChar('S');
	SSsendChar('R');
	SSsendChar('F');
	SSsendChar('1');
	SSsendChar('0');
	SSsendChar('6');
	//---- body
	SSsendChar(buf->datum);
	//---- tail
	SSsendChar('*');
	SSsendChar(ch1);
	SSsendChar(ch2);
	SSsendChar(0x0D);
	SSsendChar(0x0A);
}

// GPS Read commands



#ifdef _EXAMPLES_

// positioning

	// create a GPS data connection to GPRMC sentence type
	NMEA gps(GPRMC);

	void setup() {
		Serial1.begin(4800);
		pinMode(8, OUTPUT);
	}

	void loop() {
		if (Serial1.available() > 0 ) {
			// read incoming character from GPS
			char c = Serial1.read();

			// check if the character completes a valid GPS sentence
			if (gps.decode(c)) {
				// check if GPS positioning was active
				if (gps.gprmc_status() == 'A') {
					// check if you are in Colorado, USA
					boolean inColorado = (gps.gprmc_latitude() > 37.0)
					&& (gps.gprmc_latitude() < 41.0)
					&& (gps.gprmc_longitude() < -102.05)
					&& (gps.gprmc_longitude() > -109.05);

					// set led accordingly
					if (inColorado) {
						digitalWrite(8, HIGH);
						} else {
						digitalWrite(8, LOW);
					}
				}
			}
		}
	}
	
	
	NMEA gps(GPRMC);  // GPS data connection to GPRMC sentence type
	float d;          // relative direction to destination
	// destination coordinates in degrees-decimal
	float dest_latitude = 48.858342;
	float dest_longitude = 2.294522;

	void setup() {
		Serial1.begin(4800);
		pinMode(8, OUTPUT);
		pinMode(9, OUTPUT);
	}
// line to dest
	void loop() {
		if (Serial1.available() > 0 ) {
			// read incoming character from GPS
			char c = Serial1.read();

			// check if the character completes a valid GPS sentence
			if (gps.decode(c)) {
				// check if GPS positioning was active
				if (gps.gprmc_status() == 'A') {
					// calculate relative direction to destination
					d = gps.gprmc_course_to(dest_latitude, dest_longitude) - gps.gprmc_course();
					if (d < 0) { d += 360; }
					if (d > 180) { d -= 360; }
					// set LEDs accordingly
					if (d < 5) {
						digitalWrite(8, HIGH);
						} else {
						digitalWrite(8, LOW);
					}
					if (d > -5) {
						digitalWrite(9, HIGH);
						} else {
						digitalWrite(9, LOW);
					}
				}
			}
		}
	}
	
// Speed calc
// create a GPS data connection to GPRMC sentence type
NMEA gps(GPRMC);

void setup() {
	Serial1.begin(4800);
	pinMode(8, OUTPUT);
}

void loop() {
	if (Serial1.available() > 0 ) {
		// read incoming character from GPS
		char c = Serial1.read();

		// check if the character completes a valid GPS sentence
		if (gps.decode(c)) {
			// check if GPS positioning was active
			if (gps.gprmc_status() == 'A') {
				// read speed in kilometers-per-hour and set led accordingly
				if (gps.gprmc_speed(KMPH) > 50.0) {
					digitalWrite(8, HIGH);
					} else {
					digitalWrite(8, LOW);
				}
			}
		}
	}
}

// Distance to dest
NMEA gps(GPRMC);  // GPS data connection to GPRMC sentence type

// destination coordinates in degrees-decimal
float dest_latitude = 48.858342;
float dest_longitude = 2.294522;

void setup() {
	Serial1.begin(4800);
	pinMode(8, OUTPUT);
}

void loop() {
	if (Serial1.available() > 0 ) {
		// read incoming character from GPS
		char c = Serial1.read();

		// check if the character completes a valid GPS sentence
		if (gps.decode(c)) {
			// check if GPS positioning was active
			if (gps.gprmc_status() == 'A') {
				// read distance to destination in meters and set led accordingly
				if (gps.gprmc_distance_to(dest_latitude, dest_longitude, MTR) < 500.0) {
					digitalWrite(8, HIGH);
					} else {
					digitalWrite(8, LOW);
				}
			}
		}
	}
}

//
#endif