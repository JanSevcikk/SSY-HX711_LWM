/*
 * adconv.h
 *
 * Created: 2/27/2024 15:10:46
 *  Author: Student
 */ 


#ifndef ADCONV_H_
#define ADCONV_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define uref_AREF 0
#define uref_ACDD 1
#define uref_I1V5 2
#define uref_I1V6 3
#define prescale_div_factor4 2
#define prescale_div_factor8 3
#define prescale_div_factor16 4
#define channelADC 3
#define sourceADC 0 //free running mode

void ADC_Init(uint8_t prescale,uint8_t uref);
void ADC_Start_per(uint8_t prescale,uint8_t uref,uint8_t chan,uint8_t source);
uint16_t ADC_get(uint8_t chan);
uint16_t ADC_readTemp();
void ADC_stop(void);


#endif /* ADCONV_H_ */