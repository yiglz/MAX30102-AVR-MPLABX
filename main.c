#include "max30102.h"


uint32_t RedDataBuffer[32];
uint32_t IRDataBuffer[32];

int main() {

    I2C_Init();


    InitSettings MAX30102_Config;


    MAX30102_Config.Sample_Average = SMP_AVE_4;
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: SMP_AVE[2:0] (Samples Averaged per FIFO sample)
     * DEFINITION  | SAMPLES | DESCRIPTION
     * SMP_AVE_1   |    1    | No averaging
     * SMP_AVE_2   |    2    | 2 samples
     * SMP_AVE_4   |    4    | 4 samples (Standard)
     * SMP_AVE_8   |    8    | 8 samples
     * SMP_AVE_16  |   16    | 16 samples
     * SMP_AVE_32  |   32    | 32 samples
     * ------------------------------------------------------------------------- */


    MAX30102_Config.Interrupt_Enable = 0;
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: Interrupt Enable (3-bit)
     * VALUE | TARGET BIT | FUNCTION     | DESCRIPTION
     * 0     | None       | No Interrupt | Polling mode (Silent)
     * 1     | Bit 5      | ALC_OVF_EN   | Ambient Light Overflow
     * 2     | Bit 6      | PPG_RDY_EN   | New Data Ready
     * 4     | Bit 7      | A_FULL_EN    | FIFO Almost Full
     * ------------------------------------------------------------------------- */


    MAX30102_Config.Almost_Full = 5;
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: FIFO Almost Full (0-15)
     * VALUE | EMPTY SPACES LEFT | SAMPLES IN FIFO AT INTERRUPT
     * 0     |       0           |      32
     * 5     |       5           |      27
     * 10    |      10           |      22 
     * 15    |      15           |      17 
     * ------------------------------------------------------------------------- */


    MAX30102_Config.Mode_Select = HR_AND_SPO2_MODE;
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: Mode Configuration
     * DEFINITION       | DESCRIPTION
     * HR_MODE          | Red LED only
     * SP02_MODE        | Red + IR LEDs 
     * HR_AND_SPO2_MODE | Multi-LED Mode 
     * ------------------------------------------------------------------------- */


    MAX30102_Config.SpO2_ADC_Range = SPO2_ADC_RGE_4096;
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: SpO2 ADC Range (Sensitivity)
     * DEFINITION         | RANGE     | LSB SIZE
     * SPO2_ADC_RGE_2048  | 2048 nA   | 7.81 pA
     * SPO2_ADC_RGE_4096  | 4096 nA   | 15.63 pA
     * SPO2_ADC_RGE_8192  | 8192 nA   | 31.25 pA
     * SPO2_ADC_RGE_16384 | 16384 nA  | 62.5 pA
     * ------------------------------------------------------------------------- */


    MAX30102_Config.SpO2_Sample_Rate = SPO2_SR100;
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: SpO2 Sample Rate (Sampling Speed)
     * DEFINITION    | RATE      | DESCRIPTION
     * SPO2_SR50     | 50 Hz     | 50 samples per second
     * SPO2_SR100    | 100 Hz    | 100 samples per second (Standard)
     * SPO2_SR200    | 200 Hz    | 200 samples per second
     * SPO2_SR400    | 400 Hz    | 400 samples per second
     * SPO2_SR800    | 800 Hz    | 800 samples per second
     * SPO2_SR1000   | 1000 Hz   | 1000 samples per second
     * ------------------------------------------------------------------------- */


    MAX30102_Config.LED_Pulse_Width = LED_PW411;
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: LED Pulse Width (Resolution)
     * DEFINITION | WIDTH  | ADC RESOLUTION
     * LED_PW69   | 69 us  | 15 bit
     * LED_PW118  | 118 us | 16 bit
     * LED_PW215  | 215 us | 17 bit
     * LED_PW411  | 411 us | 18 bit
     * ------------------------------------------------------------------------- */


    MAX30102_Config.Red_LED_Current = 0x24; // ~7.2mA
    MAX30102_Config.IR_LED_Current  = 0x24; // ~7.2mA
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: LED Current Amplitudes (mA)
     * HEX VAL | CURRENT | DESCRIPTION
     * 0x00    | 0.0 mA  | Off
     * 0x24    | 7.2 mA  | 
     * 0x7F    | 25.4 mA | 
     * 0xFF    | 51.0 mA | Maximum Brightness
     * ------------------------------------------------------------------------- */

    MAX30102_Config.TemperatureEnable = DIE_TEMP_RDY_EN;
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: Die Temperature Sensor
     * VALUE           | STATUS   | DESCRIPTION
     * 0               | Disabled | Temperature sensor is off
     * DIE_TEMP_RDY_EN | Enabled  | Measures internal temperature
     * ------------------------------------------------------------------------- */

    MAX30102_Config.FIFO_Rollover = FIFO_ROLLOVER_EN;
    /* -------------------------------------------------------------------------
     * REFERENCE TABLE: FIFO Rollover (Circular Buffer)
     * DEFINITION       | STATUS   | DESCRIPTION
     * 0                | Disabled | FIFO stops when full.
     * FIFO_ROLLOVER_EN | Enabled  | New data overwrites the oldest.
     * ------------------------------------------------------------------------- */


    MAX30102_Init(MAX30102_Config);


    while (1) {

        MAX30102_FIFORead(RedDataBuffer, IRDataBuffer);
        
        __delay_ms(10); 
    }
}