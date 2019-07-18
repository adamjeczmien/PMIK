/**
 * @file flash.c
 * @brief Functions handling flash memory
 */
#include "stm32f10x.h"
#include "globalVar.h"
#define Page_127        0x0801FC00

/**
 * @brief Write into memory
 */
void FLASH_write(uint16_t* data, uint16_t size)
{
	uint16_t i;
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
	//FLASH_Erase Page
	while((FLASH->SR&FLASH_SR_BSY));
	FLASH->CR |= FLASH_CR_PER; //Page Erase Set
	FLASH->AR = Page_127; //Page Address
	FLASH->CR |= FLASH_CR_STRT; //Start Page Erase
	while((FLASH->SR&FLASH_SR_BSY));
	FLASH->CR &= ~FLASH_CR_PER; //Page Erase Clear
	//FLASH_Program HalfWord
	FLASH->CR |= FLASH_CR_PG;
	for(i=0; i<size; i++){
		while((FLASH->SR&FLASH_SR_BSY));
		*(volatile uint16_t*)(Page_127 + i*2) = data[i];
	}
	FLASH->CR &= ~FLASH_CR_PG;
	FLASH->CR |= FLASH_CR_LOCK;
}
/**
 * @brief Read from memory
 */
void FLASH_read(uint16_t* data, uint16_t size)
{
	uint16_t i;
	for(i=0; i<size; i++){
		data[i] = *(volatile uint16_t*)(Page_127 + i*2);
	}

}

/**
 * @brief Read previous trigger value from flash.
 * If there was no value start with default.
 */
void readPreviousTemperature(){
	float tempTemperature;
		uint16_t intTempTemperature=0;
		FLASH_read(&intTempTemperature, 1);
		tempTemperature=(float)intTempTemperature;
		tempTemperature=tempTemperature/10;
		if(isnan(triggerTemperature)||triggerTemperature>100){
			triggerTemperature=DEFAULTTRIGGER;
		}
		else{
			triggerTemperature=tempTemperature;
		}
}
