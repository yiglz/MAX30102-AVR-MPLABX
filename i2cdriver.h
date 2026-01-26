#ifndef I2CDRIVERH
#define I2CDRIVERH

#include <xc.h>
#include <stdint.h>

// =================================================================================================
//  I2C FUNCTION PROTOTYPES
// =================================================================================================

void I2C_Init(void);
uint8_t I2C_Start(void);
uint8_t I2C_Write(uint8_t data);
uint8_t I2C_ReadWithACK(uint8_t *dataPtr);
uint8_t I2C_ReadWithNACK(uint8_t *dataPtr);
uint8_t I2C_RepeatedStart(void);
void I2C_Stop(void);
void I2C_End(void);

#endif