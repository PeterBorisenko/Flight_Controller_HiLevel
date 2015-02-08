#include "TWI_mega328.h"
/*
 * TWI_mega328.c
 *
 * Created: 12/7/2014 1:07:44 AM
 *  Author: Disgust
 */ 
// TODO: Add ACK/NACK Support
/*
 * \brief Initializes the I2C communication peripheral.
 *
 * \param clockFreq - I2C clock frequency (Hz).
 *                    Example: 100000 - SPI clock frequency is 100 kHz.
 * \return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful;
 *                           0 - if initialization was unsuccessful.
 */
uint8_t TWI_Init(uint64_t clockFreq) // TODO: Complete this Init function
{
    uint8_t   status      = 1;
    uint64_t   pbFrequency = 80000000;
    uint16_t  brgValue    = 0;

//    I2C1CON = 0;                /*!< Clear the content of I2C1CON register */
    /*! Fsck = Fpb / ((I2CxBRG + 2) * 2) */
    brgValue = pbFrequency / (2 * clockFreq) - 2;
//    I2C1BRG = brgValue;
	
    DDRC&= ~((1 << PINC4)|(1 << PINC5));    // Configure TWI Pins as inputs
    PORTC|= (1 << PINC4)|(1 << PINC5);      // Connect internal PULL-UPs
	
    return status;
}

/**
  * \ Sets start condition
  * 
  * 
  * \return void
  */
uint8_t TWIstart() {
     TWCR= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);    // Send START condition
     while (!BIT_read(TWCR, TWINT));                 // Waiting for START condition transmits
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != START) {                  // Check for transmission status (masking TWI prescaler bits)
         return 0;
     }
#endif
	return 1;
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

uint8_t TWIslaveWrite(uint8_t addr) {
    TWDR= ((addr << 1) | TWI_WRITE);                                    // Send command
    TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
    while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
    if ((TWSTA & 0xF8) != MT_SLA_ACK) {
        return 0;
    }
#endif
	return 1;
 }

uint8_t TWIslaveRead(uint8_t addr) {
    TWDR= ((addr << 1) | TWI_READ);                                    // Send command
    TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
    while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
    if ((TWSTA & 0xF8) != MR_SLA_ACK) {
        return 0;
    }
#endif
	return 1;
 }

uint8_t TWIbyteWrite(uint8_t dat) {
    TWDR= dat;
    TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
    while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
    if ((TWSTA & 0xF8) != MT_DATA_ACK) {
        return 0;
    }
#endif
	return 1;
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
        return 0;
    }
#endif
    return 0;
}


 /**
  * \brief 
  * 
  * \param addr - Slave Address
  * \param dat - Pointer to buffer consists data to write
  * \param num - Number of bytes to write
  * 
  * \return void
  */
uint8_t TWIwrite(uint8_t addr, uint8_t * dat, uint8_t num)
 {
     TWCR= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);    // Send START condition
     while (!BIT_read(TWCR, TWINT));                 // Waiting for START condition transmits
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != START) {                  // Check for transmission status (masking TWI prescaler bits)
         return 0;
     }
#endif
     TWDR= ((addr << 1) | TWI_WRITE);                                    // Send command
     TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
     while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != MT_SLA_ACK) {
         return 0;
     }
#endif
     for (int i= num; i > 0; i--) {
         TWDR= *dat;
         TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
         while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
         if ((TWSTA & 0xF8) != MT_DATA_ACK) {
             return 0;
         }
#endif
         dat++;
     }
     TWIstop();   // Send STOP condition
	 return 1;
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
uint8_t TWIread(uint8_t addr, uint8_t * dat, uint8_t num)
 {
     TWCR= (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);    // Send START condition
     while (!BIT_read(TWCR, TWINT));                 // Waiting for START condition transmits
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != START) {                  // Check for transmission status (masking TWI prescaler bits)
         return 0;
     }
#endif
     TWDR= ((addr << 1) | TWI_READ);                                    // Send command
     TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
     while (!BIT_read(TWCR, TWINT));
#ifndef WITHOUT_CHECKS
     if ((TWSTA & 0xF8) != MR_SLA_ACK) {
         return 0;
     }
#endif
     for (int i= num; i > 0; i--) {
         TWCR|= (1 << TWINT)|(1 << TWEN);                // Clear TWINT to start transmission
         while (!BIT_read(TWCR, TWINT));
         if ((TWSTA & 0xF8) == MR_DATA_ACK) {
             *dat = TWDR;
         }
#ifndef WITHOUT_CHECKS
         else {
             return 0;
         }
#endif
         dat++;
     }
     TWIstop();
	 return 1;
 }