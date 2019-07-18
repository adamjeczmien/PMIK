/**
 * @file uartLib.h
 * @brief UART Library header
 */
#ifndef UARTLIB_H_
#define UARTLIB_H_

void send_char(char c);
void send_string(const char* s);
void changeTemperature(const char* newtemperature);
void reciveTemperature(void);
void uartAction(void);
int __io_putchar(int c);
void sendInstruction(void);

#endif /* UARTLIB_H_ */
