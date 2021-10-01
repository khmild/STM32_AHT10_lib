/**
 * @file aht10_lib.cpp
 * @author Denys Khmil
 * @brief This file contents all the aht10 library functions
 */
#include "aht10_lib.h"

/**
 * @brief display1602 constructor
 * @param _i2c: I2C instance.
 * @param _address: Sensor address.
 */
aht10::aht10(I2C_HandleTypeDef *_i2c, uint8_t _address){
    /*Save i2c instsnce and sensor address*/
    this->i2c = _i2c;
    this->address = _address;
    /*Initialize the sensor*/
    this->Initialize();
}

/**
 * @brief display1602 constructor
 * @param _i2c: I2C instance.
 */
aht10::aht10(I2C_HandleTypeDef *_i2c){
    /*Save i2c instsnce and sensor address*/
    this->i2c = _i2c;
    this->address = 0x38;
    /*Initialize the sensor*/
    this->Initialize();
}

/**
 * @brief Soft reset for the AHT10 sensor
 */
void aht10::reset(){
    uint8_t buf = SOFT_RESET;
    HAL_I2C_Master_Transmit(this->i2c, (this->address << 1), &buf, 1, 1000);
}

/**
 * @brief Get humidity function
 * @retval Humidity in %
 */
float aht10::getHumid(){
    uint8_t buffer[4];
    /*Trigger measurement*/
    this->Measure();
    /*Wait for measurement to finish*/
    while(this->isBusy());
    /*Read data from sensor*/
    HAL_I2C_Master_Receive(this->i2c, (this->address << 1), buffer, 4, 1000);
    /*Convert values*/
    uint32_t humid;
    humid = (buffer[1] << 12)|(buffer[2] << 4)|(buffer[3] >> 4);
    return (humid/1048576.0)*100.0;
}

/**
 * @brief Get temperature function
 * @retval Temperature in C 
 */
float aht10::getTemp(){
    uint8_t buffer[6];
    /*Trigger measurement*/
    this->Measure();
    /*Wait for measurement to finish*/
    while(this->isBusy());
    /*Read data from sensor*/
    HAL_I2C_Master_Receive(this->i2c, (this->address << 1), buffer, 6, 1000);
    /*Convert values*/
    uint32_t temp;
    buffer[3] = buffer[3]&15;
    temp = ((uint32_t)buffer[3] << 16)|((uint32_t)buffer[4] << 8)|(buffer[5]);
    return ((temp/1048576.0)*200.0)-50.0;
}

/**
 * @brief Check if sensor is busy
 * @retval 1 - busy, 0 - ready
 */
uint8_t aht10::isBusy(){
    uint8_t data;
    /*Read status register*/
    HAL_I2C_Master_Receive(this->i2c, (this->address << 1), &data, 1, 1000);
    
    if(data&(1 << 7)) return 1;
    else return 0;
}

/**
 * @brief Sends command to trigger the measurement
 */
void aht10::Measure(){
    uint8_t buffer[3];
    buffer[0] = TRIG_MEASUREMENT;
    buffer[1] = 0x33;
    buffer[2] = 0;
    HAL_I2C_Master_Transmit(this->i2c, (this->address << 1), buffer, 3, 1000);
}

/**
 * @brief Sends command to initialize the sensor
 */
void aht10::Initialize(){
    uint8_t buf[3];
    buf[0] = INIT_COMMAND;
    buf[1] = 0x08;
    buf[2] = 0;
    HAL_I2C_Master_Transmit(this->i2c, (this->address << 1), buf, 3, 1000);
}