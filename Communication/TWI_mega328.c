#include "TWI_mega328.h"
/*
 * TWI_mega328.c
 *
 * Created: 12/7/2014 1:07:44 AM
 *  Author: Disgust
 */ 

// TWSR values (not bits)

// Master
#define TW_START					0x08
#define TW_REP_START				0x10
// Master Transmitter
#define TW_MT_SLA_ACK				0x18
#define TW_MT_SLA_NACK				0x20
#define TW_MT_DATA_ACK				0x28
#define TW_MT_DATA_NACK				0x30
#define TW_MT_ARB_LOST				0x38
// Master Receiver
#define TW_MR_ARB_LOST				0x38
#define TW_MR_SLA_ACK				0x40
#define TW_MR_SLA_NACK				0x48
#define TW_MR_DATA_ACK				0x50
#define TW_MR_DATA_NACK				0x58
// Slave Transmitter
#define TW_ST_SLA_ACK				0xA8
#define TW_ST_ARB_LOST_SLA_ACK		0xB0
#define TW_ST_DATA_ACK				0xB8
#define TW_ST_DATA_NACK				0xC0
#define TW_ST_LAST_DATA				0xC8
// Slave Receiver
#define TW_SR_SLA_ACK				0x60
#define TW_SR_ARB_LOST_SLA_ACK		0x68
#define TW_SR_GCALL_ACK				0x70
#define TW_SR_ARB_LOST_GCALL_ACK	0x78
#define TW_SR_DATA_ACK				0x80
#define TW_SR_DATA_NACK				0x88
#define TW_SR_GCALL_DATA_ACK		0x90
#define TW_SR_GCALL_DATA_NACK		0x98
#define TW_SR_STOP					0xA0
// Misc
#define TW_NO_INFO					0xF8
#define TW_BUS_ERROR				0x00

// defines and constants
#define TWCR_CMD_MASK		0x0F
#define TWSR_STATUS_MASK	0xF8

// return values
#define I2C_OK				0x00
#define I2C_ERROR_NODEV		0x01

/*
 * \brief Initializes the I2C communication peripheral.
 *
 * \param clockFreq - I2C clock frequency (Hz).
 *                    Example: 100000 - SPI clock frequency is 100 kHz.
 * \return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful;
 *                           0 - if initialization was unsuccessful.
 */
unsigned char TWI_Init(unsigned long clockFreq)
{
    unsigned char   status      = 1;
    unsigned long   pbFrequency = 80000000;
    unsigned short  brgValue    = 0;

//    I2C1CON = 0;                /*!< Clear the content of I2C1CON register */
    /*! Fsck = Fpb / ((I2CxBRG + 2) * 2) */
    brgValue = pbFrequency / (2 * clockFreq) - 2;
//    I2C1BRG = brgValue;
	
    DDRC&= ~((1 << PINC4)|(1 << PINC5));    // Configure TWI Pins as inputs
    PORTC|= (1 << PINC4)|(1 << PINC5);      // Connect internal PULL-UPs
	
    return status;
}

// Low-level commands

/**
  * \ Sets start condition
  * 
  * 
  * \return void
  */
void TWIstart() {
     TWCR= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);    // Send START condition
     while (!BIT_read(TWCR, TWINT));                 // Waiting for START condition transmits
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != START) {                  // Check for transmission status (masking TWI prescaler bits)
         //error(TWI);
     }
#endif
 }

/**
 * \ Sets stop condition
 * 
 * 
 * \return void
 */
void TWIstop() {
    TWCR|= (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);   // Send STOP condition
}

void TWIslaveWrite(uint8_t addr) {
    TWDR= ((addr << 1) | WRITE);                                    // Send command
    TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
    while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
    if ((TWSTA & 0xF8) != MT_SLA_ACK) {
        //error(TWI);
    }
#endif
 }

void TWIslaveRead(uint8_t addr) {
    TWDR= ((addr << 1) | READ);                                    // Send command
    TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
    while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
    if ((TWSTA & 0xF8) != MR_SLA_ACK) {
        //error(TWI);
    }
#endif
 }

void TWIbyteWrite(uint8_t dat) {
    TWDR= dat;
    TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
    while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
    if ((TWSTA & 0xF8) != MT_DATA_ACK) {
        //error(TWI);
    }
#endif
}

uint8_t TWIbyteRead()
{
	// TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
	while (!BIT_read(TWCR, TWINT));

	if ((TWSTA & 0xF8) == MR_DATA_ACK) {
    	return TWDR;
	}

#ifndef WITHOUT_CHECKS
    else {
        //error(TWI);
        return 0;
    }
#endif
    return 0;
}


// Hi-level commands

 /**
  * \brief 
  * 
  * \param addr - Slave Address
  * \param dat - Pointer to buffer consists data to write
  * \param num - Number of bytes to write
  * 
  * \return void
  */
 void TWIwrite(uint8_t addr, uint8_t * dat, uint8_t num)
 {
     TWCR= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);    // Send START condition
     while (!BIT_read(TWCR, TWINT));                 // Waiting for START condition transmits
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != START) {                  // Check for transmission status (masking TWI prescaler bits)
         //error(TWI);
     }
#endif
     TWDR= ((addr << 1) | WRITE);                                    // Send command
     TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
     while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != MT_SLA_ACK) {
         //error(TWI);
     }
#endif
     for (int i= num; i >= 0; i--) {
         TWDR= *dat;
         TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
         while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
         if ((TWSTA & 0xF8) != MT_DATA_ACK) {
             //error(TWI);
         }
#endif
         dat++;
     }
     TWCR|= (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);   // Send STOP condition
 }

 /**
  * \ Reads number of bytes
  * 
  * \param addr - Slave Address
  * \param dat - Pointer to buffer for save received data
  * \param num - Number of bytes to read
  * 
  * \return void
  */
void TWIread(uint8_t addr, uint8_t * dat, uint8_t num)
 {
     TWCR= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);    // Send START condition
     while (!BIT_read(TWCR, TWINT));                 // Waiting for START condition transmits
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != START) {                  // Check for transmission status (masking TWI prescaler bits)
         //error(TWI);
     }
#endif
     TWDR= ((addr << 1) | READ);                                    // Send command
     TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
     while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != MR_SLA_ACK) {
         //error(TWI);
     }
#endif
     for (int i= num; i >= 0; i--) {
         TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
         while (!BIT_read(TWCR, TWINT));
         if ((TWSTA & 0xF8) == MR_DATA_ACK) {
             *dat = TWDR;
         }
#ifndef WITHOUT_CHECKS
         else {
             //error(TWI);
         }
#endif
         dat++;
     }
     TWIstop();
 }