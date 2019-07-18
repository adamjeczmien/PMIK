/**
 * @file flash.h
 * @brief Flash functions header
 */
#ifndef FLASH_H_
#define FLASH_H_
void FLASH_write(uint16_t* data, uint16_t size);

void FLASH_read(uint16_t* data, uint16_t size);

void readPreviousTemperature(void);

#endif /* FLASH_H_ */
