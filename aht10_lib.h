/**
 * @file aht10_lib.h
 * @author Denys Khmil
 * @brief This file contents the aht10 class
 */
#ifndef AHT10_LIB
#define AHT10_LIB

#include "main.h"

typedef enum{
    INIT_COMMAND = 0xe1,
    TRIG_MEASUREMENT = 0xac,
    SOFT_RESET = 0xba
}AHT10_commands;

class aht10{
    public:
    aht10(I2C_HandleTypeDef *_i2c, uint8_t _address);
    aht10(I2C_HandleTypeDef *_i2c);
    void reset();
    float getHumid();
    float getTemp();
    uint8_t isBusy();
    void setMode();
    
    private:
    void Measure();
    void Initialize();
    I2C_HandleTypeDef *i2c;
    uint8_t address;

};

#endif