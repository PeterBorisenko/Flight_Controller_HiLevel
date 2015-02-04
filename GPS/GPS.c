/*
 * GPS.c
 *
 * Created: 04.02.2015 0:07:57
 *  Author: Disgust
 */ 

#include "GPS.h"

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

void GPSsetSerialPort(GPScommand100_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
				(allBitsXor(buf->baud[gpsBaudLength]) << 3)|(allBitsXor(buf->dataBits) << 2)|(allBitsXor(buf->stopBits) << 1)|(allBitsXor(buf->parity));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	sendChar('$');
	sendChar('P');
	sendChar('S');
	sendChar('R');
	sendChar('F');
	sendChar('1');
	sendChar('0');
	sendChar('0');
	//---- body
	sendChar(buf->proto);
	for (uint8_t i= 0; i < gpsBaudLength; i++)
	{
		sendChar(buf->baud[i]);
	}
	sendChar(buf->dataBits);
	sendChar(buf->stopBits);
	sendChar(buf->parity);
	//---- tail
	sendChar('*');
	sendChar(ch1);
	sendChar(ch2);
	sendChar(0x0D);
	sendChar(0x0A);
}

void GPSinitNav(GPScommand101_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor((uint8_t)buf->weekNo >> 8) << 3)|(allBitsXor((uint8_t)buf->weekNo) << 2)|(allBitsXor(buf->chnlCount) << 1)|(allBitsXor(buf->resetCfg));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	sendChar('$');
	sendChar('P');
	sendChar('S');
	sendChar('R');
	sendChar('F');
	sendChar('1');
	sendChar('0');
	sendChar('1');
	//---- body
	sendChar((uint8_t)buf->posX >> 24); // TODO: check this!
	sendChar((uint8_t)buf->posX >> 16);
	sendChar((uint8_t)buf->posX >> 8); 
	sendChar((uint8_t)buf->posX);
	sendChar((uint8_t)buf->posY >> 24);
	sendChar((uint8_t)buf->posY >> 16);
	sendChar((uint8_t)buf->posY >> 8);
	sendChar((uint8_t)buf->posY);
	sendChar((uint8_t)buf->posZ >> 24);
	sendChar((uint8_t)buf->posZ >> 16);
	sendChar((uint8_t)buf->posZ >> 8);
	sendChar((uint8_t)buf->posZ);
	sendChar((uint8_t)buf->clkOffset >> 24);
	sendChar((uint8_t)buf->clkOffset >> 16);
	sendChar((uint8_t)buf->clkOffset >> 8);
	sendChar((uint8_t)buf->clkOffset);
	sendChar((uint8_t)buf->timeOfWeek >> 24);
	sendChar((uint8_t)buf->timeOfWeek >> 16);
	sendChar((uint8_t)buf->timeOfWeek >> 8);
	sendChar((uint8_t)buf->timeOfWeek);
	sendChar((uint8_t)buf->weekNo >> 8);
	sendChar((uint8_t)buf->weekNo);
	sendChar(buf->chnlCount);
	sendChar(buf->resetCfg);
	//---- tail
	sendChar('*');
	sendChar(ch1);
	sendChar(ch2);
	sendChar(0x0D);
	sendChar(0x0A);
}

void GPSsetDGPS(GPScommand102_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor('2') << 3)|(allBitsXor(buf->dataBits) << 2)|(allBitsXor(buf->stopBits) << 1)|(allBitsXor(buf->parity));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	sendChar('$');
	sendChar('P');
	sendChar('S');
	sendChar('R');
	sendChar('F');
	sendChar('1');
	sendChar('0');
	sendChar('2');
	//---- body
	sendChar(buf->dataBits);
	sendChar(buf->stopBits);
	sendChar(buf->parity);
	//---- tail
	sendChar('*');
	sendChar(ch1);
	sendChar(ch2);
	sendChar(0x0D);
	sendChar(0x0A);
}

void GPSsetQueryRateCtl(GPScommand103_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor(buf->msg) << 3)|(allBitsXor(buf->mode) << 2)|(allBitsXor(buf->rate) << 1)|(allBitsXor(buf->checkSumEn));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	sendChar('$');
	sendChar('P');
	sendChar('S');
	sendChar('R');
	sendChar('F');
	sendChar('1');
	sendChar('0');
	sendChar('3');
	//---- body
	sendChar(buf->msg);
	sendChar(buf->mode);
	sendChar(buf->rate);
	sendChar(buf->checkSumEn);
	//---- tail
	sendChar('*');
	sendChar(ch1);
	sendChar(ch2);
	sendChar(0x0D);
	sendChar(0x0A);
}

void GPSllaNavInit(GPScommand104_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
				(allBitsXor((uint8_t)buf->weekNo >> 8) << 3)|(allBitsXor((uint8_t)buf->weekNo) << 2)|(allBitsXor(buf->channelCount) << 1)|(allBitsXor(buf->resetCfg));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	sendChar('$');
	sendChar('P');
	sendChar('S');
	sendChar('R');
	sendChar('F');
	sendChar('1');
	sendChar('0');
	sendChar('4');
	//---- body
	sendChar((uint8_t)buf->lat >> 24); // TODO: Check this!!!
	sendChar((uint8_t)buf->lat >> 16);
	sendChar((uint8_t)buf->lat >> 8);
	sendChar((uint8_t)buf->lat);
	sendChar((uint8_t)buf->lon >> 24);
	sendChar((uint8_t)buf->lon >> 16);
	sendChar((uint8_t)buf->lon >> 8);
	sendChar((uint8_t)buf->lon);
	sendChar((uint8_t)buf->alt >> 24);
	sendChar((uint8_t)buf->alt >> 16);
	sendChar((uint8_t)buf->alt >> 8);
	sendChar((uint8_t)buf->alt);
	sendChar((uint8_t)buf->clkOffset >> 24);
	sendChar((uint8_t)buf->clkOffset >> 16);
	sendChar((uint8_t)buf->clkOffset >> 8);
	sendChar((uint8_t)buf->clkOffset);
	sendChar((uint8_t)buf->timeOfWeek >> 24);
	sendChar((uint8_t)buf->timeOfWeek >> 16);
	sendChar((uint8_t)buf->timeOfWeek >> 8);
	sendChar((uint8_t)buf->timeOfWeek);
	sendChar((uint8_t)buf->weekNo >> 8);
	sendChar((uint8_t)buf->weekNo);
	sendChar(buf->channelCount);
	sendChar(buf->resetCfg);
	//---- tail
	sendChar('*');
	sendChar(ch1);
	sendChar(ch2);
	sendChar(0x0D);
	sendChar(0x0A);
}

void GPStglDevData(GPScommand105_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor('1') << 3)|(allBitsXor('0') << 2)|(allBitsXor('5') << 1)|(allBitsXor(buf->debug));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	sendChar('$');
	sendChar('P');
	sendChar('S');
	sendChar('R');
	sendChar('F');
	sendChar('1');
	sendChar('0');
	sendChar('5');
	//---- body
	sendChar(buf->debug);
	//---- tail
	sendChar('*');
	sendChar(ch1);
	sendChar(ch2);
	sendChar(0x0D);
	sendChar(0x0A);
}

void GPSselDatum(GPScommand106_t * buf) {
	uint8_t ch= (allBitsXor('P') << 7)|(allBitsXor('S') << 6)|(allBitsXor('R') << 5)|(allBitsXor('F') << 4)|\
	(allBitsXor('1') << 3)|(allBitsXor('0') << 2)|(allBitsXor('6') << 1)|(allBitsXor(buf->datum));

	uint8_t ch1= HEX_TO_ASCII(HI(ch));
	uint8_t ch2= HEX_TO_ASCII(LO(ch));
	//---- head
	sendChar('$');
	sendChar('P');
	sendChar('S');
	sendChar('R');
	sendChar('F');
	sendChar('1');
	sendChar('0');
	sendChar('6');
	//---- body
	sendChar(buf->datum);
	//---- tail
	sendChar('*');
	sendChar(ch1);
	sendChar(ch2);
	sendChar(0x0D);
	sendChar(0x0A);
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