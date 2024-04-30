/*
 * at30tse758.h
 *
 * Created: 2/27/2024 14:25:02
 *  Author: Student
 */ 


#ifndef AT30TSE758_H_
#define AT30TSE758_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define TempSensorAddrR 0b10010111		//adresy vcetne R/W bitu, pro jednoduchost...
#define TempSensorAddrW 0b10010110
#define Temp_tempRegister 0x00
#define Temp_configRegister 0x01
#define Temp_lowLimRegister 0x02
#define Temp_highLimRegister 0x03

#define SerialEEPROMP3AddrR 0b10100111
#define SerialEEPROMP3AddrW 0b10100110
#define SerialEEPROMP2AddrR 0b10100101
#define SerialEEPROMP2AddrW 0b10100100
#define SerialEEPROMP1AddrR 0b10100011
#define SerialEEPROMP1AddrW 0b10100010
#define SerialEEPROMP0AddrR 0b10100001
#define SerialEEPROMP0AddrW 0b10100000

#define OS 15
#define R1 14
#define R0 13
#define FT1 12
#define FT0 11
#define POL 10
#define CMPINT 9
#define SD 9
#define NVRBSY 0


uint8_t at30_setPrecision(uint8_t prec);
float at30_readTemp(void);
uint8_t at30_readPrecision(void);


#endif /* AT30TSE758_H_ */