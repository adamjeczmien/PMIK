/**
 * @file    interrupt.c
 * @brief File defining interruptions used in project
 */
#include "stm32f10x.h"
#include "interrupt.h"
#include "globalVar.h"
#include "pinconfig.h"

/**
 * @brief Timer triggering temperature measurement
 */
void TIM3_IRQHandler(){
	if(!changingTemperature){
		GPIO_SetBits(HEARTBEATPORT, HEARTBEATPIN);
	}
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
			uint16_t newadc = ADC_GetConversionValue(ADC1);
			float fadc=newadc;
			float range=4095;
			float voltage = fadc/range*3.3;
			float r=4610*((range/fadc)-1);
			currentTemperature=(15560-r)/219.4613;
			if(voltage>0.1){
			gcvt(currentTemperature,3,currentTemperatureBuff);
			lcd=1;
			}
	}
	if(!changingTemperature){
		GPIO_ResetBits(HEARTBEATPORT, HEARTBEATPIN);
	}
}

/**
 * @brief Timer triggering fan start or stop
 */
void TIM4_IRQHandler(){
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		GPIO_ResetBits(OUTPORT, OUTPIN);
		GPIO_ResetBits(OUT2PORT, OUT2PIN);
		GPIO_ResetBits(OUT3PORT, OUT3PIN);
			if((triggerTemperature+6)<currentTemperature){
				GPIO_SetBits(OUTPORT, OUTPIN);
				GPIO_SetBits(OUT2PORT, OUT2PIN);
				GPIO_SetBits(OUT3PORT, OUT3PIN);
			}
			else if((triggerTemperature+3)<currentTemperature){
				GPIO_SetBits(OUTPORT, OUTPIN);
				GPIO_SetBits(OUT2PORT, OUT2PIN);
			}
			else if(triggerTemperature<currentTemperature){
				GPIO_SetBits(OUTPORT, OUTPIN);
			}
			else{

			}
	}
}
/**
 * @brief Reset LCD screen
 */
void EXTI15_10_IRQHandler()
{
 if (EXTI_GetITStatus(EXTI_Line13)) {
	 if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0) {
		 LCD_Init();
	 }

 EXTI_ClearITPendingBit(EXTI_Line13);
 }
}
