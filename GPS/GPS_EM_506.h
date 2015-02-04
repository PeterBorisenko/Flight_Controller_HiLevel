/*
 * GPS.h
 * Globalsat EM-406 GPS Module driver
 * Created: 04.02.2015 0:06:47
 *  Author: Disgust
 */ 


#ifndef GPS_EM_506_H_
#define GPS_EM_506_H_

// General ops
#define GPS_BAUD	4800
#define PARITY_NONE	DEC_TO_ASCII(0)
#define PARITY_ODD	DEC_TO_ASCII(1)
#define PARITY_EVEN	DEC_TO_ASCII(2)
#define USE_CHKSUM	DEC_TO_ASCII(1)

// protocols
#define PROTO_SIRF		DEC_TO_ASCII(0)
#define PROTO_NMEA		DEC_TO_ASCII(1)
#define PROTO_USER1		DEC_TO_ASCII(4)

// reset configs
#define DV_WARM_HOT_ST	DEC_TO_ASCII(1)
#define CE_WARM_ST		DEC_TO_ASCII(2)
#define CM_COLD_ST		DEC_TO_ASCII(4)

// msg types
#define GGA				DEC_TO_ASCII(0)
#define GLL				DEC_TO_ASCII(1)
#define	GSA				DEC_TO_ASCII(2)
#define GSV				DEC_TO_ASCII(3)
#define RMC				DEC_TO_ASCII(4)
#define VTG				DEC_TO_ASCII(5)

// debug
#define GPS_DEBUG_OFF		DEC_TO_ASCII(0)
#define GPS_DEBUG_ON		DEC_TO_ASCII(1)

// datum

// Position Fix Indicator
#define FIX_NA	DEC_TO_ASCII(0)	// Fix not available or invalid
#define SPS_FV	DEC_TO_ASCII(1)	// GPS SPS Mode, fix valid
#define DIFF_FV	DEC_TO_ASCII(2)	// Differential GPS, SPS Mode , fix valid
#define PPS_FV	DEC_TO_ASCII(3)	// GPS PPS Mode, fix valid 

// Indicators
#define WEST	'W'
#define EAST	'E'
#define NORD	'N'
#define SOUTH	'S'

// Data status
#define DATA_VALID		'A'
#define DATA_INVALID	'V'

// GSA Mode 1
#define MAN_MODE1	'M' // Manual-forced to operate in 2D or 3D mode
#define AUTO_MODE1	'A'	// 2D automatic-allowed to automatically switch 2D/3D

// GSA Mode 2
#define NO_FIX_MODE2	DEC_TO_ASCII(0) // Fix Not Available
#define _2D_MODE2		DEC_TO_ASCII(1)	// 2D
#define _3D_MODE2		DEC_TO_ASCII(2)	// 3D

// baud selection
#if GPS_BAUD == 1200
	char GPSbaud[]=	"1200";
	uint8_t gpsBaudLength= 4;
#elif GPS_BAUD == 2400
	char GPSbaud[]=	"2400";
	uint8_t gpsBaudLength= 4;
#elif GPS_BAUD == 4800
	char GPSbaud[]=	"4800";
	uint8_t gpsBaudLength= 4;
#elif GPS_BAUD == 9600
	char GPSbaud[]=	"9600";
	uint8_t gpsBaudLength= 4;
#elif GPS_BAUD == 19200
	char GPSbaud[]=	"19200";
	uint8_t gpsBaudLength= 5;
#elif GPS_BAUD == 38400
	char GPSbaud[]=	"38400";
	uint8_t gpsBaudLength= 5;
#endif


// NMEA Input Commands

// Set Serial Port 
typedef struct {
	uint8_t		proto;
	uint8_t		dataBits;
	uint8_t		stopBits;
	uint8_t		parity;
} GPSsetSerialPort_t;

// Navigation initialization
typedef struct {
	int32_t		posX;
	int32_t		posY;
	int32_t		posZ;
	int32_t		clkOffset;
	uint32_t	timeOfWeek;
	uint16_t	weekNo;
	uint8_t		chnlCount;
	uint8_t		resetCfg;
} GPSinitNav_t;

// Set DGPS
typedef struct {
	uint8_t		dataBits; // always 8
	uint8_t		stopBits;
	uint8_t		parity;
} GPSsetDGPS_t;

// Query/Rate Control
typedef struct {
	uint8_t		msg;
	uint8_t		mode;
	uint8_t		rate;
	uint8_t		checkSumEn;
} GPSsetQueryRateCtl_t;

// LLA Navigation initialization 
typedef struct {
	float		lat;
	float		lon;
	float		alt;
	int32_t		clkOffset;
	uint32_t	timeOfWeek;
	uint16_t	weekNo;
	uint8_t		channelCount;
	uint8_t		resetCfg;
} GPSinitLlaNav_t;

// Development Data On/Off 
typedef struct {
	uint8_t		debug;
} GPStglDevData_t;

// Select Datum
typedef struct {
	uint8_t		datum;
} GPSselDatum_t;

// Write functions prototypes
void GPSsetSerialPort(GPSsetSerialPort_t *);

void GPSinitNav(GPSinitNav_t *);

void GPSsetDGPS(GPSsetDGPS_t *);

void GPSsetQueryRateCtl(GPSsetQueryRateCtl_t *);

void GPSllaNavInit(GPSinitLlaNav_t *);

void GPStglDevData(GPStglDevData_t *);

void GPSselDatum(GPSselDatum_t *);

// NMEA Receive Data Structured

typedef struct {
	// GGA-Global Positioning System Fixed Data
	// Msg ID $GPGGA
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t millis;
	float	lat;
	uint8_t	N_S;
	float	lon;
	uint8_t	E_W;
	uint8_t	posFix;
	uint8_t	satsUsed;
	float	HDOP;	// Horizontal Dilution of Precision
	float	MSLalt;
	uint8_t	MSLaltUnits;
	float	geoidSep;
	uint8_t	geoidSepUnits;
	uint8_t	ageOfDiffCorr;
	uint16_t	diffRefStaID;
} GPS_GGA_MSG_t;

typedef struct {
	// GLL-Geographic Position-Latitude/Longitude
	// Msg ID $GPGLL
	float	lat;
	uint8_t	N_S;
	float	lon;
	uint8_t	E_W;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t millis;
	uint8_t	satsUsed;
	uint8_t dataStatus;
} GPS_GLL_MSG_t;

typedef struct {
	// GSA Data Format
	// Msg ID $GPGSA
	uint8_t mode1;
	uint8_t mode2;
	uint8_t satsUsedCh1;
	uint8_t satsUsedCh2;
	uint8_t satsUsedCh3;
	uint8_t satsUsedCh4;
	uint8_t satsUsedCh5;
	uint8_t satsUsedCh6;
	uint8_t satsUsedCh7;
	uint8_t satsUsedCh8;
	uint8_t satsUsedCh9;
	uint8_t satsUsedCh10;
	uint8_t satsUsedCh11;
	uint8_t satsUsedCh12;
	float	PDOP;	// Position dilution of Precision
	float	HDOP;	// Horizontal dilution of Precision
	float	VDOP;	// Vertical dilution of Precision
} GPS_GSA_MSG_t;

typedef struct {
	// GSV Data Format
	// Msg ID $GPGSV
	uint8_t numOfMsg;	// Range 1 to 3
	uint8_t msgNum;		// Range 1 to 3
	//channel 1
	uint8_t satsInViewCh1;
	uint8_t satIDCh1;
	uint8_t elevationCh1;
	uint8_t azimuthCh1;
	uint8_t snrCh1;
	// channel 2
	uint8_t satsInViewCh2;
	uint8_t satIDCh2;
	uint8_t elevationCh2;
	uint8_t azimuthCh2;
	uint8_t snrCh2;
	//channel 3
	uint8_t satsInViewCh3;
	uint8_t satIDCh3;
	uint8_t elevationCh3;
	uint8_t azimuthCh3;
	uint8_t snrCh3;
	//channel 4
	uint8_t satsInViewCh4;
	uint8_t satIDCh4;
	uint8_t elevationCh4;
	uint8_t azimuthCh4;
	uint8_t snrCh4;
} GPS_GSV_MSG_t;

typedef struct {
	// RMC Data Format
	// Msg ID $GPRMC
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t millis;
	uint8_t dataStatus;
	float	lat;
	uint8_t	N_S;
	float	lon;
	uint8_t	E_W;
	float groundSpeed;
	float groundCourse;
	uint8_t day;
	uint8_t mounth;
	uint8_t year;
	uint8_t magneticVar; // Does not supported by SiRF
} GPS_RMC_MSG_t;

typedef struct {
	// VTG Data Format
	// Msg ID $GPVTG
	float	courseT;
	//uint8_t ref; T - true
	float	courseM;
	//uint8_t ref; M - magnetic
	float speedN;
	//uint8_t units; N - knots
	float speedK;
	//uint8_t units; K - Km/h
	
} GPS_VTG_MSG_t;



#endif /* GPS_EM_506_H_ */