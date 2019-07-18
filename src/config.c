/**
 * @file    config.c
 * @brief File containing startup functions
 * for the I/O pins, clocks and modules used in project
 */
#include "stm32f10x.h"
#include "config.h"
#include "pinconfig.h"
/**
 * @brief Configuration of the timers
 */
void TIMERS_Config(void){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


	TIM_TimeBaseInitTypeDef TIM_TimeBaseMili_InitStructure;
	TIM_TimeBaseMili_InitStructure.TIM_Prescaler = 64000 - 1;
	TIM_TimeBaseMili_InitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseMili_InitStructure);


	TIM_TimeBaseInitTypeDef TIM_TimeBaseMicro_InitStructure;
	TIM_TimeBaseMicro_InitStructure.TIM_Prescaler = 35 - 1;
	TIM_TimeBaseMicro_InitStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseMicro_InitStructure);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseADC_InitStructure;
	TIM_TimeBaseADC_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseADC_InitStructure.TIM_Prescaler = 64000 - 1;
	TIM_TimeBaseADC_InitStructure.TIM_Period = MEASTIMEPERIODINMILISECONDS - 1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseADC_InitStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseTrigger_InitStructure;
	TIM_TimeBaseTrigger_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseTrigger_InitStructure.TIM_Prescaler = 64000 - 1;
	TIM_TimeBaseTrigger_InitStructure.TIM_Period = COMPARISONTIMEPERIODINMILISECONDS - 1;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseTrigger_InitStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}
/**
 * @brief Configuration of the analog to
 * digital converter
 */
void ADC_Config(void){

	ADC_InitTypeDef adc;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_StructInit(&adc);
	adc.ADC_ContinuousConvMode = ENABLE;
	adc.ADC_NbrOfChannel = 1;
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init(ADC1, &adc);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_Cmd(ADC1, ENABLE);


	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
 * @brief Configurations of the interruptions
 */
void INTERRUPT_Config(void){
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = TIM3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = TIM4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 0x00;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

/**
 * @brief Input\Output pins configuration
 */
void GPIO_Config(void){
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = HEARTBEATPIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(HEARTBEATPORT, &gpio);

	gpio.GPIO_Pin = OUTPIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OUTPORT, &gpio);

	gpio.GPIO_Pin = OUT2PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OUT2PORT, &gpio);

	gpio.GPIO_Pin = OUT3PIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OUT3PORT, &gpio);

	gpio.GPIO_Pin = UARTTXPIN;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UARTTXPORT, &gpio);

	gpio.GPIO_Pin = UARTRXPIN;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UARTRXPORT, &gpio);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = EXTI_Line13;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);
}

/**
 * @brief Configuration of the USART
 */
void UART_Config(void){
	USART_InitTypeDef uart;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_StructInit(&uart);
	uart.USART_BaudRate = 9600;
	USART_Init(USART2, &uart);
	USART_Cmd(USART2, ENABLE);
}

