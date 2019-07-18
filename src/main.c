/**
 * @file    main.c
 * @brief Main function.
 * Launch initializing func and startup.
 */
#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "common.h"
#include "config.h"
#include "lcd.h"
#include "uartLib.h"
#include "globalVar.h"
#include "interrupt.h"
#include "pinconfig.h"
#include "flash.h"

int main(void)
{
	readPreviousTemperature();
	GPIO_Config();
	UART_Config();

	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = ADCINPIN;
	gpio.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADCINPORT, &gpio);

	TIMERS_Config();
	LCD_Init();
	INTERRUPT_Config();
	ADC_Config();
	sendInstruction();
	lcdSayHello();
	while (1) {
		if(lcd==1){
			lcdShowTemperature();
		}
		if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
			if(!changingTemperature){
				uartAction();
			}
		}
	}
}
