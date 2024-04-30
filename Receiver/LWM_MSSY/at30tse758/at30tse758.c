/*
 * at30tse758.c
 *
 * Created: 2/27/2024 14:25:15
 *  Author: Student
 */ 

#include "../makra.h"
#include "at30tse758.h"

// zapis do konf. registru
uint8_t at30_setPrecision(uint8_t prec){
	uint_fast16_t config_register=0;
	config_register |= (uint16_t)(prec << R0);
	i2cStart();
	i2cWrite(TempSensorAddrW);
	if(i2cGetStatus() != 0x18){
		UART_SendString("Error 18\n\r");
		return 0;
	}
	i2cWrite(Temp_configRegister);
	if (i2cGetStatus() != 0x28){
		UART_SendString("Error 28\n\r");
		return 0;
	}
	i2cWrite((uint8_t)(config_register>>8));
	if (i2cGetStatus() != 0x28){
		UART_SendString("Error 28\n\r");
		return 0;
	}
	i2cWrite((uint8_t)(config_register));
	if (i2cGetStatus() != 0x28){
		UART_SendString("Error 28\n\r");
		return 0;
	}
	i2cStop();
	return 1;
}


float at30_readTemp(void){
	volatile uint8_t buffer[2];
	volatile int16_t teplotaTMP;
	
	i2cStart();
	i2cWrite(TempSensorAddrW);
	if(i2cGetStatus() != 0x18){
		UART_SendString("Error 18\n\r");
	}
	i2cWrite(Temp_tempRegister);
	if (i2cGetStatus() != 0x28){
		UART_SendString("Error 28\n\r");
	}
	i2cStop();
	
	i2cStart();
	if(i2cGetStatus() != 0x08){
		UART_SendString("Error 08\n\r");
	}
	i2cWrite(TempSensorAddrR);
	if(i2cGetStatus() != 0x40){
		UART_SendString("Error 40\n\r");
	}
	buffer[0]=i2cReadACK();
	if(i2cGetStatus() != 0x50){
		UART_SendString("Error 50\n\r");
	}
	buffer[1]=i2cReadNACK();
	if(i2cGetStatus() != 0x58){
		UART_SendString("Error 58\n\r");
	}
	i2cStop();
	
	teplotaTMP=(buffer[0]<<8)|buffer[1];
	return (float)teplotaTMP/256;
	
}
uint8_t at30_readPrecision(void){
	volatile uint8_t buffer;
	volatile uint8_t prec;
	
	i2cStart();
	i2cWrite(TempSensorAddrW);
	if(i2cGetStatus() != 0x18){
		UART_SendString("Error 18\n\r");
	}
	i2cWrite(Temp_configRegister);
	if (i2cGetStatus() != 0x28){
		UART_SendString("Error 28\n\r");
	}
	i2cStop();
	
	i2cStart();
	i2cWrite(TempSensorAddrR);
	if(i2cGetStatus() != 0x40){
		UART_SendString("Error 40\n\r");
	}
	buffer=i2cReadNACK();
	if(i2cGetStatus() != 0x58){
		UART_SendString("Error 58\n\r");
	}
	i2cStop();
	prec = (buffer & 0x60) >> 5;
	return (uint8_t)prec;
}
