#include "max30102.h"
#include <avr/fuse.h>

FUSES = {
    .low = 0xFF,
    .high = 0xD9,
    .extended = 0xFF,
};

// -----------------------------------------------------------------------------
//  MAX30102 WRITE FUNCTION
// -----------------------------------------------------------------------------

void MAX30102_Write(uint8_t RegAddress, uint8_t data) {
    I2C_Start();
    I2C_Write(MAX30102_Write_Address);
    I2C_Write(RegAddress);
    I2C_Write(data);
    I2C_Stop();
}

// -----------------------------------------------------------------------------
//  MAX30102 REGISTER READ FUNCTION
// -----------------------------------------------------------------------------

uint8_t MAX30102_RegisterRead(uint8_t address) {
    I2C_Start();
    I2C_Write(MAX30102_Write_Address);
    I2C_Write(address);
    I2C_RepeatedStart();
    I2C_Write(MAX30102_Read_Address);
    uint8_t data;
    I2C_ReadWithNACK(&data);
    I2C_Stop();
    return data;
} 

// -----------------------------------------------------------------------------
//  MAX30102 INITIALIZATION FUNCTION
// -----------------------------------------------------------------------------

void MAX30102_Init(InitSettings settings) {

    // Reseting all configs and datas to 0
    MAX30102_Write(ModeConfig, 1 << RST);
    __delay_ms(100);
    
    // Reseting the Pointers and the Counter
    MAX30102_Write(0x04, 0x00);
    MAX30102_Write(0x05, 0x00);
    MAX30102_Write(0x06, 0x00);

    // FIFO Configuration
    MAX30102_Write(FIFOConfig, (0xE0 & (settings.Sample_Average << 5)) | 
                               (0x10 & (settings.FIFO_Rollover << 4 )) |
                               (0x0F & (settings.Almost_Full << 0)));

    // Interrupt Enable
    MAX30102_Write(INTEnable, (0xE0 & (settings.Interrupt_Enable << 5)));

    // SpO2 Configuration
    MAX30102_Write(SPO2Config, (0x60 & (settings.SpO2_ADC_Range << 5)) | 
                               (0x1C & (settings.SpO2_Sample_Rate << 2 )) |
                               (0x03 & (settings.LED_Pulse_Width << 0)));

    // LED Pulse Amplitude
    MAX30102_Write(REG_LED1_PA, settings.Red_LED_Current);
    MAX30102_Write(REG_LED2_PA, settings.IR_LED_Current);

    // Mode Configuration
    MAX30102_Write(ModeConfig, (0x07 & (settings.Mode_Select << 0)));

    // Die Temperature Configuration
    MAX30102_Write(DieTempConfig, (settings.TemperatureEnable << 0));
}


// -----------------------------------------------------------------------------
//  MAX30102 DATA READ FUNCTION
// -----------------------------------------------------------------------------


void MAX30102_FIFORead(uint32_t* RedData, uint32_t* IRData) {
    uint8_t RD_PTR_LOCATION;
    uint8_t WR_PTR_LOCATION;
    
    WR_PTR_LOCATION = MAX30102_RegisterRead(FIFO_WR_PTR_REG);
    RD_PTR_LOCATION = MAX30102_RegisterRead(FIFO_RD_PTR_REG);
    
    int8_t NUM_AVAILABLE_SAMPLES = WR_PTR_LOCATION - RD_PTR_LOCATION;
    uint8_t NUM_SAMPLES_TO_READ;
    
    NUM_SAMPLES_TO_READ = NUM_AVAILABLE_SAMPLES;
    
    // Buffer Wrap-Around
    if(NUM_AVAILABLE_SAMPLES < 0) {
        NUM_SAMPLES_TO_READ = NUM_AVAILABLE_SAMPLES + 32;
    }

    if (NUM_SAMPLES_TO_READ == 0) {
        return;
    }
    
    I2C_Start();
    I2C_Write(MAX30102_Write_Address);
    I2C_Write(FIFO_DATA);
    I2C_RepeatedStart();
    
    uint8_t redByte1 = 0x00;
    uint8_t redByte2 = 0x00;
    uint8_t redByte3 = 0x00;
    uint8_t IRByte1  = 0x00;
    uint8_t IRByte2  = 0x00;
    uint8_t IRByte3  = 0x00;
    
    I2C_Write(MAX30102_Read_Address);
    
    for (uint8_t i = 0; i < NUM_SAMPLES_TO_READ; i++) {
        // Read Red LED Data
        I2C_ReadWithACK(&redByte1);
        I2C_ReadWithACK(&redByte2);
        I2C_ReadWithACK(&redByte3);
        *(RedData + i) = ((uint32_t)redByte1 << 16 | (uint32_t)redByte2 << 8 | (uint32_t)redByte3) & 0x3FFFF;
        
        // Read IR LED Data
        I2C_ReadWithACK(&IRByte1);
        I2C_ReadWithACK(&IRByte2);
        
        if (i != NUM_SAMPLES_TO_READ - 1) {
            I2C_ReadWithACK(&IRByte3);
        } else {
            I2C_ReadWithNACK(&IRByte3);
        }
        *(IRData + i) = ((uint32_t)IRByte1 << 16 | (uint32_t)IRByte2 << 8 | (uint32_t)IRByte3) & 0x3FFFF;
    }
    I2C_Stop();
    
    // Sync FIFO Pointers
    I2C_Start();
    I2C_Write(MAX30102_Write_Address);
    I2C_Write(FIFO_RD_PTR_REG);
    I2C_Write(WR_PTR_LOCATION);
    I2C_Stop();
}

