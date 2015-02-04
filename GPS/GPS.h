/*
 * GPS.h
 * Globalsat EM-406 GPS Module driver
 * Created: 04.02.2015 0:06:47
 *  Author: Disgust
 */ 


#ifndef GPS_H_
#define GPS_H_
 


#define GPS_BAUD	4800
#define PARITY_NONE	0x00
#define PARITY_ODD	0x01
#define PARITY_EVEN	0x02
#define USE_CHKSUM	0x01
// protocols
#define PROTO_SIRF		0x00
#define PROTO_NMEA		0x01
#define PROTO_USER1		0x04

// reset configs
#define DV_WARM_HOT_ST	0x01
#define CE_WARM_ST		0x02
#define CM_COLD_ST		0x04

// msg types
#define GGA				0x00
#define GLL				0x01
#define	GSA				0x02
#define GSV				0x03
#define RMC				0x04
#define VTG				0x05

// debug
#define GPS_DEBUG_OFF		0x00
#define GPS_DEBUG_ON		0x01

// datum


// command IDs
char setSerialId[]=	"100";
char navInitId[]=	"101";
char setDGPS[]=		"102";
char queryRateCtl[]="103";
char LLAnavInit[]=	"104";
char devData[]=		"105";
char selDatum[]=	"106";

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
} GPScommand100_t;

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
} GPScommand101_t;

// Set DGPS
typedef struct {
	uint8_t		dataBits; // always 8
	uint8_t		stopBits;
	uint8_t		parity;
} GPScommand102_t;

// Query/Rate Control
typedef struct {
	uint8_t		msg;
	uint8_t		mode;
	uint8_t		rate;
	uint8_t		checkSumEn;
} GPScommand103_t;

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
} GPScommand104_t;

// Development Data On/Off 
typedef struct {
	uint8_t		debug;
} GPScommand105_t;

// Select Datum
typedef struct {
	uint8_t		datum;
} GPScommand106_t;

// Config function prototypes
void GPSsetSerialPort(GPScommand100_t *);

void GPSinitNav(GPScommand101_t *);

void GPSsetDGPS(GPScommand102_t *);

void GPSsetQueryRateCtl(GPScommand103_t *);

void GPSllaNavInit(GPScommand104_t *);

void GPStglDevData(GPScommand105_t *);

void GPSselDatum(GPScommand106_t *);


#endif /* GPS_H_ */