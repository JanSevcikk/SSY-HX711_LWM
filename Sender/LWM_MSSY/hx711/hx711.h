/*
 * hx711.h
 *
 * Created: 3/18/2024 14:24:17
 *  Author: Student
*/


#ifndef HX711_H_
#define HX711_H_




void HX711_start();
void HX711_reset();
unsigned long HX711_getZERO();
unsigned long HX711_measure();
unsigned long HX711_measureAvg10();
unsigned long HX711_measureAvg25();
unsigned long HX711_measureGrams();
//void calibration0Kg();
void calibration1Kg();
//void calibration2Kg(); //nepouzito

#endif /* HX711_H_ */  
