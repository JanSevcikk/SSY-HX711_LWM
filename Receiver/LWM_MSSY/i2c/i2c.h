/*
 * i2c.h
 *
 * Created: 2/27/2024 14:04:50
 *  Author: Student
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "../makra.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void i2cInit(void);
void i2cStart(void);
void i2cStop(void);
void i2cWrite(uint8_t u8data);
uint8_t i2cReadACK(void);
uint8_t i2cReadNACK(void);
uint8_t i2cGetStatus(void);






#endif /* I2C_H_ */