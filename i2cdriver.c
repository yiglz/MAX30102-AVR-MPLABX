
#include "i2cdriver.h"

// =================================================================================================
//  I2C INITIALIZATION
// =================================================================================================
/* SCL Frequency Calculation:
 * SCL_freq = F_CPU / (16 + 2 * (TWBR * Prescaler))
 * * For F_CPU = 8MHz and TWBR = 32 (0x20):
 * SCL = 8,000,000 / (16 + 2 * (32 * 1)) = 100 kHz (Standard Mode)
*/

void I2C_Init(void) {
    TWBR  =   0x20; // Setting the bit rate to 100KHz
    TWSR &=   ~((1<<TWPS1) | (1<<TWPS0)); // Prescaler=1
}

// =================================================================================================
//  I2C START
// =================================================================================================

uint8_t I2C_Start(void){

    TWCR  = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));//TWINT Flag clear, setting Start and enabling TWI
    while (!(TWCR & (1<<TWINT))); // Waiting for TWINT Flag set
    return (TWSR & 0XF8); //Status code return
}

// =================================================================================================
//  I2C WRITE
// =================================================================================================

uint8_t I2C_Write(uint8_t data) {

    TWDR = data; // Loading data to Data Register
    TWCR  = ((1<<TWINT) | (1<<TWEN)); // Starting transmission

    while (!(TWCR & (1<<TWINT)));// Waiting for TWINT Flag set
    return (TWSR & 0XF8);//Status code return
}

// =================================================================================================
//  I2C READ WITH ACK
// =================================================================================================

uint8_t I2C_ReadWithACK(uint8_t *dataPtr) {

TWCR  = ((1<<TWINT) | (1<<TWEA) | (1<<TWEN));// Enable Acknowledge (TWEA) to tell slave to send next byte
while (!(TWCR & (1<<TWINT))); 
    *dataPtr = TWDR; // Recieving the data
    return (TWSR & 0XF8);//Status code return
}

// =================================================================================================
//  I2C READ WITH NACK
// =================================================================================================

uint8_t I2C_ReadWithNACK(uint8_t *dataPtr) {

    TWCR  = ((1<<TWINT) | (1<<TWEN)); // Not enabling ACK, telling slave to stop transmission

    while (!(TWCR & (1<<TWINT)));
    *dataPtr = TWDR; // Recieving the data
    return (TWSR & 0XF8);//Status code return
}

// =================================================================================================
//  I2C REPEATED START
// =================================================================================================

uint8_t I2C_RepeatedStart(void) {
    
    TWCR  = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));
    while (!(TWCR & (1<<TWINT)));
    return (TWSR & 0XF8);//Status code return
}

// =================================================================================================
//  I2C STOP
// =================================================================================================

void I2C_Stop(void) {
    TWCR  = ((1<<TWINT) | (1<<TWSTO) | (1<<TWEN)); // Clear TWINT, send STOP condition (TWSTO), enable TWI (TWEN)
}

// =================================================================================================
//  I2C END
// =================================================================================================

//Disables the TWI peripheral. Use this to save power when I2C is no longer needed.
void I2C_End(void) {
    TWCR &= ~(1<<TWEN); 
}