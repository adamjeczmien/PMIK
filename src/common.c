/**
 * @file    common.c
 * @brief Common delay functions
 */
#include "stm32f10x.h"
#include "common.h"

/**
 * @brief Delay system for the 'czas' milliseconds
 */
void delay_ms(uint16_t czas) {

	TIM_SetCounter(TIM2,czas);
	TIM_Cmd(TIM2, ENABLE);
 	while (TIM_GetCounter(TIM2));
	TIM_Cmd(TIM2, DISABLE);
}
/**
 * @brief Delay system for the 'czas' microseconds
 */
void delay_us(uint8_t czas) {

	TIM_SetCounter(TIM1,czas);
	TIM_Cmd(TIM1, ENABLE);
	while (TIM_GetCounter(TIM1));
	TIM_Cmd(TIM1, DISABLE);
}


