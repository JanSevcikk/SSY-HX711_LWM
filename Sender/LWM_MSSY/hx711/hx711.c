/*
 * HX711711.c
 *
 * Created: 3/18/2024 14:24:03
 *  Author: Student
 */

#include <util/delay.h>
#include <stdio.h>
#include "hx711.h"
#include "../makra.h"

unsigned long HX711_ZERO = 0;
unsigned long HX711_averageCount = 0;
unsigned long HX711_GRAMS = 0;
unsigned long HX711_kal = 0;
//Nekonecne checkuj zmenu a pricitej zmerene

void HX711_start(){
	
			HX711_ZERO = HX711_measureAvg25();
			printf("\r\nHX711_ZERO hodnota nastavena na: %ld\r\n",HX711_ZERO);
			return;
		
		
	}

unsigned long HX711_getZERO(){
	if(HX711_ZERO == 0){
		HX711_start();
	}
	return (long)HX711_ZERO;
	
}
unsigned long HX711_setZERO(){
	if(HX711_ZERO == 0){
		HX711_start();
	}
	return (long)HX711_ZERO;
	
}

void HX711_reset(){
	//Sck na HIGH po dobu v�c jak 60us => reset ?ipu 
	printf("\n\r Prevodnik resetovan");
	sbi(PORTB,2);
	_delay_us(70);
	cbi(PORTB,2);
	
}

unsigned long HX711_measure(){
	while(tbi(PINB,3));
	unsigned long count = 0;
	for (int i=0;i<24;i++)
		{
			
			sbi(PORTB,2);
			_delay_us(1);
			count = count<<1;
			cbi(PORTB,2);
			if(tbi(PINB,3)) count++;
			_delay_us(1);
		}
	sbi(PORTB,2);
	_delay_us(1);
	count = count^0x800000; // je potreba
	cbi(PORTB,2);
	return count;
}

unsigned long HX711_measureAvg10(){
	unsigned long pocet = 0;
	unsigned long sumCount = 0;
	while(1){
		sumCount = sumCount + HX711_measure();
		pocet++;
		if(pocet == 10){
			HX711_averageCount = sumCount/pocet;
			pocet = 0;
			sumCount = 0;
			printf("\r\nPrumerZERO: %ld\r\n",HX711_averageCount);
			return (long)HX711_averageCount;
		}
		
	}
}

unsigned long HX711_measureAvg25(){
	unsigned long pocet = 0;
	unsigned long sumCount = 0;
	while(1){
		sumCount = sumCount + HX711_measure();
		pocet++;
		if(pocet == 25){
			HX711_averageCount = sumCount/pocet;
			pocet = 0;
			sumCount = 0;
			printf("\r\nPrumerZERO: %ld\r\n",HX711_averageCount);
			return (long)HX711_averageCount;
		}
		
	}
}



unsigned long HX711_measureGrams(){
	HX711_measureAvg10();
	
	if(HX711_averageCount >= HX711_ZERO){
		if(HX711_kal == 0){
			HX711_GRAMS = (HX711_averageCount-HX711_ZERO)/16;
		}else{
			HX711_GRAMS = (HX711_averageCount-HX711_ZERO)/HX711_kal;
		}
		
	}else{
		HX711_GRAMS = 0;
	}
	
	printf("\r\nZmereno: %ld\r\n",HX711_GRAMS);
	return HX711_GRAMS;

	
}
/*
void calibration0Kg(){
	printf("\r\nVloz 0kg znovu zmackni 0: %ld\r\n");
	while (tbi(PINB, 4));
	HX711_reset();
	HX711_start();
	// Po stisknut� tla?�tka 1 provede kalibraci
	//HX711711_start();

	// Potvrzen� kalibrace
	printf("\r\nKalibrovano na 0kg. kal hodnota je= %ld\r\n",HX711_kal);
}
*/
void calibration1Kg(){
	printf("\r\nVloz 1kg znovu zmackni 1: %ld\r\n");
	// ?ek� na stisknut� tla?�tka 1
	while (tbi(PINE, 5));
	HX711_kal = (HX711_measureAvg10()-HX711_getZERO())/1000; //vzorky / d?leno z�kladn� base hodnotou a d?leno kilem
	// Po stisknut� tla?�tka 1 provede kalibraci
	//HX711711_start();

	// Potvrzen� kalibrace
	printf("\r\nKalibrovano na 1kg. kal hodnota je= %ld\r\n",HX711_kal);
}
/*
void calibration2Kg(){
	printf("\r\nVloz 2kg znovu zmackni 2: %ld\r\n");
	while (tbi(PINF, 0));
	HX711_kal = (HX711_measureAvg10()-HX711_getZERO())/2000;
	// Po stisknut� tla?�tka 1 provede kalibraci
	//HX711711_start();

	// Potvrzen� kalibrace
	printf("\r\nKalibrovano na 2kg. kal hodnota je= %ld\r\n",HX711_kal);
}
*/
