/**
 * @file    uartLib.c
 * @brief All functions used to communicate via USART.
 */
#include "stm32f10x.h"
#include "globalVar.h"
#include "pinconfig.h"
#include "uartLib.h"
#include <stdlib.h>
#include <stdlib.h>

/**
 * @brief Send single char.
 */
void send_char(char c)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, c);
}
/**
 * @brief Override used if 'printf' via USART is used.
 */
int __io_putchar(int c)
{
	if (c=='\n')
		send_char('\r');
	send_char(c);
	return c;
}
/**
 * @brief Send strings of chars.
 */
void send_string(const char* s)
{
	while (*s)
		send_char(*s++);
}

/**
 * @brief Save new temperature value set by the user.
 */
void changeTemperature(const char* newtemperature){
	char buff[3];
	float temp=atof(newtemperature);
	if(temp>100){
		triggerTemperature=100;
		send_string("\r\nTemperature cannot be higher than 100\r\n");
		send_string("New trigger temperature: 100*C\r\n");
	}
	else if(temp<-10){
		triggerTemperature=-10;
		send_string("\r\nTemperature cannot be lower than -10\r\n");
		send_string("New trigger temperature: -10*C\r\n");
	}
	else{
		triggerTemperature=temp;
		send_string("\r\nNew trigger temperature: ");
		gcvt(temp,3,buff);
		send_string(buff);
		send_string("*C\r\n");
	}
	float tempTemperature;
	uint16_t intTempTemperature;
	tempTemperature=10*triggerTemperature;
	intTempTemperature=(int)tempTemperature;
	FLASH_write(&intTempTemperature, 1);
}
/**
 * @brief receive new temperature value
 */
void receiveTemperature(void){
	GPIO_SetBits(HEARTBEATPORT, HEARTBEATPIN);
	changingTemperature=1;
	int licz=0;
	int isANumber;
	triggerTemperature=DEFAULTTRIGGER;
	char buff[3];
	while(licz<=3){
		if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)){
			char c=USART_ReceiveData(USART2);
			if(c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||c=='7'||c=='8'||c=='9'||c=='0'||c=='.'||c=='-'){
				isANumber=1;
			}
			else if(c=='x'){
				send_char(c);
				break;
			}
			else{
				isANumber=0;
			}
			if(isANumber){
				send_char(c);
				buff[licz]=c;
				licz++;
			}
			else{
				send_string("Use Numbers\r\n");
				sendInstruction();
				gcvt(DEFAULTTRIGGER,3,buff);
				break;
			}
		}
	}
	changeTemperature(buff);
	changingTemperature=0;
	lcd=1;
	GPIO_ResetBits(HEARTBEATPORT, HEARTBEATPIN);
}

/**
 * @brief Define what action was chosen by user
 */
void uartAction(void){
	char ch = USART_ReceiveData(USART2);
	int newadc;
	switch (ch)
	{
	 case 'c':
		 send_string("Changing Temperature\r\n");
		 send_string("To terminate number press 'x'\r\n");
		 receiveTemperature();
	break;

	case 'i':
 	 	newadc=ADC_GetConversionValue(ADC1);
 		float fadc=newadc;
 		float range=4095;
 		float voltage = fadc/range*3.3;
 		float r=4610*((range/fadc)-1);
 		float temperature=(15560-r)/219.4613;
 		char buff[3];
 		send_string("ADC = ");
 		gcvt((double)newadc,4,buff);
 		send_string(buff);
 		send_string(" V = ");
 		gcvt((double)voltage,4,buff);
 		send_string(buff);
 		send_string(" T = ");
 		gcvt((double)temperature,3,buff);
 		send_string(buff);
 		send_string("'C\r\n");
		break;
	default:
		send_string("\r\nUNKNOWN COMMAND\r\n");
		sendInstruction();
		break;
	}
}
/**
 * @brief Send user manual info
 */
void sendInstruction(void){
	send_string("\r\nAvailable commands:\r\n");
	send_string("c-change trigger temperature\r\n");
	send_string("i-show data from ADC\r\n");
}



