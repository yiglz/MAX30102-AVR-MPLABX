
#ifndef MAX30102H
#define MAX30102H
#define _XTAL_FREQ 8000000
#include "i2cdriver.h"

/* --------------------------------------------------------------------------
 * I2C COMMUNICATION ADDRESSES (Already shifted for read/write)
 * -------------------------------------------------------------------------- */
#define MAX30102_Write_Address  0xAE
#define MAX30102_Read_Address   0xAF

/* --------------------------------------------------------------------------
 * REGISTER MAP
 * -------------------------------------------------------------------------- */

// --- Status and Interrupt Registers ---
#define IntStatusReg            0x00
#define IntStatusReg2           0x01
#define INTEnable               0x02
#define INTEnable2              0x03

// --- FIFO Registers ---
#define FIFO_WR_PTR_REG         0x04
#define OVF_COUNTER_REG         0x05
#define FIFO_RD_PTR_REG         0x06
#define FIFO_DATA               0x07
#define FIFO_MASK               0x1F        // 5-bit mask

// --- Configuration Registers ---
#define FIFOConfig              0x08
#define ModeConfig              0x09
#define SPO2Config              0x0A
#define REG_LED1_PA             0x0C        // Red LED Current
#define REG_LED2_PA             0x0D        // IR LED Current

// --- Multi-LED Mode Control ---
#define MultiLedModeControlReg1 0x11
#define MultiLedModeControlReg2 0x12

// --- Die Temperature Sensor ---
#define TempData                0x1F
#define DieTempFraction         0x20
#define DieTempConfig           0x21

/* --------------------------------------------------------------------------
 * BIT AND VALUE DEFINITIONS
 * -------------------------------------------------------------------------- */

// [0x00] Interrupt Status
#define A_FULL                  7           // FIFO Almost Full
#define PPG_RDY                 6           // New Data Ready
#define ALC_OVF                 5           // Ambient Light Cancellation Overflow
#define PWR_RDY                 0           // Power Ready

// [0x01] Status 2
#define DIE_TEMP_RDY            1           // Temperature Data Ready

// [0x02] Interrupt Enable
#define A_FULL_EN               7
#define PPG_RDY_EN              6
#define ALC_OVF_EN              5

// [0x03] Interrupt Enable 2
#define DIE_TEMP_RDY_EN         1

// [0x08] FIFO Configuration
#define SMP_AVE_1               0
#define SMP_AVE_2               1
#define SMP_AVE_4               2
#define SMP_AVE_8               3
#define SMP_AVE_16              4
#define SMP_AVE_32              5
#define FIFO_ROLLOVER_EN        1

// [0x09] Mode Configuration
#define SHDN                    7           // Shutdown Mode
#define RST                     6           // Soft Reset
#define HR_MODE                 2           // Heart Rate Only Mode
#define SP02_MODE               3           // HR + SpO2 Mode
#define HR_AND_SPO2_MODE        7           // Multi-LED Mode

// [0x0A] SpO2 and ADC Configuration
#define SPO2_ADC_RGE_2048       0
#define SPO2_ADC_RGE_4096       1
#define SPO2_ADC_RGE_8192       2
#define SPO2_ADC_RGE_16384      3

#define SPO2_SR50               0           // Sample Rates (Hz)
#define SPO2_SR100              1
#define SPO2_SR200              2
#define SPO2_SR400              3
#define SPO2_SR800              4
#define SPO2_SR1000             5
#define SPO2_SR1600             6
#define SPO2_SR3200             7

#define LED_PW69                0x00        // Pulse Width (Microseconds)
#define LED_PW118               0x01
#define LED_PW215               0x02
#define LED_PW411               0x03

// [0x11-0x12] Multi-LED Slot Definitions
#define SLOT1_RED               1
#define SLOT1_IR                2
#define SLOT2_RED               1
#define SLOT2_IR                2
#define SLOT3_RED               1
#define SLOT3_IR                2
#define SLOT4_RED               1
#define SLOT4_IR                2

/* --------------------------------------------------------------------------
 * Structures and Prototypes
 * -------------------------------------------------------------------------- */

typedef struct {
    uint8_t Sample_Average;     // Number of samples to average (SMP_AVE_X)
    uint8_t Interrupt_Enable;   // Interrupt authorization (PPG_RDY_EN etc.)
    uint8_t Almost_Full;        // FIFO fill threshold
    uint8_t Mode_Select;        // Operational mode (SP02_MODE etc.)
    uint8_t SpO2_ADC_Range;     // ADC full scale voltage range
    uint8_t SpO2_Sample_Rate;   // Sampling rate in Hz
    uint8_t LED_Pulse_Width;    // LED pulse width duration
    uint8_t Red_LED_Current;    // Current for Red LED (mA)
    uint8_t IR_LED_Current;     // Current for Infrared LED (mA)
    uint8_t TemperatureEnable;  // Enable/Disable temperature sensor
    uint8_t FIFO_Rollover;      // Enable FIFO pointer rollover
} InitSettings;

// --- Function Prototypes ---
void    MAX30102_Init(InitSettings settings);
void    MAX30102_Write(uint8_t RegAddress, uint8_t data);
uint8_t MAX30102_RegisterRead(uint8_t address);
void    MAX30102_FIFORead(uint32_t* RedData, uint32_t* IRData);

#endif