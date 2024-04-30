/*
 * LWM_MSSY.c
 *
 * Created: 6.4.2017 15:42:46
 * Author : Krajsa
 */ 

#include <avr/io.h>
/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "sysTimer.h"
#include "halBoard.h"
#include "halUart.h"
#include "makra.h"
#include "ADConv/adconv.h"
#include "i2c/i2c.h"
#include "makra.h"
#include "at30tse758/at30tse758.h"
#include "uart/uart.h"
#include "mat/mat.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>


/*- Definitions ------------------------------------------------------------*/
#ifdef NWK_ENABLE_SECURITY
#define APP_BUFFER_SIZE     (NWK_MAX_PAYLOAD_SIZE - NWK_SECURITY_MIC_SIZE)
#else
#define APP_BUFFER_SIZE     NWK_MAX_PAYLOAD_SIZE
#endif
#define BAUD 38400
/*- Types ------------------------------------------------------------------*/
typedef enum AppState_t
{
	APP_STATE_INITIAL,
	APP_STATE_IDLE,
} AppState_t;

/*- Prototypes -------------------------------------------------------------*/
static void appSendData(void);
static void appSendOK(uint16_t);
//static void appSendTemp(char*);


/*- Variables --------------------------------------------------------------*/
static AppState_t appState = APP_STATE_INITIAL;
static SYS_Timer_t appTimer;
static SYS_Timer_t myTimer;
static NWK_DataReq_t appDataReq;
static bool appDataReqBusy = false;
static uint8_t appDataReqBuffer[APP_BUFFER_SIZE];
static uint8_t appUartBuffer[APP_BUFFER_SIZE];
static uint8_t appUartBufferPtr = 0;
//test
static uint8_t appDataToSendIndex = 0;
static uint8_t appDataToSendSize = 0;
//static uint32_t measurementCount = 0;
FILE uart_str = FDEV_SETUP_STREAM(printCHAR, NULL, _FDEV_SETUP_RW);
//test

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
static void appDataConf(NWK_DataReq_t *req)
{
appDataReqBusy = false;
(void)req;
}

/*************************************************************************//**
*****************************************************************************/
static void appSendData(void)
{
	
if (appDataReqBusy || 0 == appUartBufferPtr)
return;

memcpy(appDataReqBuffer, appUartBuffer, appUartBufferPtr);

appDataReq.dstAddr = 1-APP_ADDR;
appDataReq.dstEndpoint = APP_ENDPOINT;
appDataReq.srcEndpoint = APP_ENDPOINT;
appDataReq.options = NWK_OPT_ENABLE_SECURITY;
appDataReq.data = appDataReqBuffer;
appDataReq.size = appUartBufferPtr;
appDataReq.confirm = appDataConf;
NWK_DataReq(&appDataReq);

appUartBufferPtr = 0;
appDataReqBusy = true;
}
//ACK OK
static void appSendOK(uint16_t target){
	if (appDataReqBusy)
	return;
	
	static char ack[]="OK\r\n\0";
	memcpy(appDataReqBuffer, ack, sizeof(appDataReqBuffer));
	

	appDataReq.dstAddr = target;
	appDataReq.dstEndpoint = APP_ENDPOINT_2;
	appDataReq.srcEndpoint = APP_ENDPOINT_2;
	appDataReq.options = NWK_OPT_ENABLE_SECURITY;
	appDataReq.data = appDataReqBuffer; //ok;
	appDataReq.size = sizeof(ack); //3;
	appDataReq.confirm = appDataConf;
	NWK_DataReq(&appDataReq);

	appDataReqBusy = true;
}

/*************************************************************************//**
*****************************************************************************/
void HAL_UartBytesReceived(uint16_t bytes)
{

	
	  for (uint16_t i = 0; i < bytes; i++)
    {
        uint8_t byte = HAL_UartReadByte();

        if (byte == '\r') // Detekce znaku Enter
        {
            HAL_UartWriteByte('\r'); // P?íslušný znak pro návrat vozíku
            HAL_UartWriteByte('\n'); // Nový ?ádek
        }
        else
        {
			if (appUartBufferPtr == sizeof(appUartBuffer))
			appSendData();
			
            if (appUartBufferPtr < sizeof(appUartBuffer))
                appUartBuffer[appUartBufferPtr++] = byte;
        }
    }

    SYS_TimerStop(&appTimer);
    SYS_TimerStart(&appTimer);
}

/*************************************************************************//**
*****************************************************************************/
static void appTimerHandler(SYS_Timer_t *timer)
{

	if (appUartBufferPtr > 0) {
        appDataToSendIndex = 0;
        appDataToSendSize = appUartBufferPtr;
        appSendData();
    }
    (void)timer;
	
}

/*************************************************************************//**
*****************************************************************************/
static bool appDataInd(NWK_DataInd_t *ind) 
{
	if(ind->data[0] == 'O' && ind->data[1] == 'K'){
		for (uint8_t i = 0; i < ind->size; i++)
		HAL_UartWriteByte(ind->data[i]);
		}else{

		for (uint8_t i = 0; i < ind->size; i++)
		HAL_UartWriteByte(ind->data[i]);
		appSendOK(ind->srcAddr);
	}

	return true;

	return true;
}

/*************************************************************************//**
*****************************************************************************/
static void appInit(void)
{
NWK_SetAddr(APP_ADDR);
NWK_SetPanId(APP_PANID);
PHY_SetChannel(APP_CHANNEL);
#ifdef PHY_AT86RF212
PHY_SetBand(APP_BAND);
PHY_SetModulation(APP_MODULATION);
#endif
PHY_SetRxState(true);

NWK_OpenEndpoint(APP_ENDPOINT, appDataInd);

NWK_OpenEndpoint(APP_ENDPOINT_2, appDataInd);

HAL_BoardInit();

appTimer.interval = APP_FLUSH_TIMER_INTERVAL;
appTimer.mode = SYS_TIMER_INTERVAL_MODE;
appTimer.handler = appTimerHandler;

myTimer.interval = MY_TIMER_INTERVAL;
myTimer.mode = SYS_TIMER_PERIODIC_MODE;
//myTimer.handler = appMyTimerHandler;
SYS_TimerStart(&myTimer);

}

/*************************************************************************//**
*****************************************************************************/
static void APP_TaskHandler(void)
{
switch (appState)
{
case APP_STATE_INITIAL:
{
appInit();
appState = APP_STATE_IDLE;
} break;

case APP_STATE_IDLE:
break;

default:
break;
}
}

/*************************************************************************//**
*****************************************************************************/
int main(void)
{
	UART_init(BAUD,1); //nastaveni rychlosti UARTu, 38400b/s
stdout = &uart_str; //presmerovani STDOUT
SYS_Init();
HAL_UartInit(38400);
HAL_UartWriteByte('z');

while (1)
{
	SYS_TaskHandler();
	HAL_UartTaskHandler();
	APP_TaskHandler();
}
}
