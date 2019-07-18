/**
 * @file pinconfig.h
 * @brief File containing port names and pin numbers
 */

#define HEARTBEATPIN 						GPIO_Pin_9
#define	HEARTBEATPORT						GPIOC

/**
 * @brief Analog/Digital Converter
 */
#define ADCINPIN							GPIO_Pin_0
#define ADCINPORT							GPIOA

/**
 * @brief UART Transmission
 */
#define UARTRXPIN							GPIO_Pin_3
#define UARTRXPORT							GPIOA
#define UARTTXPIN							GPIO_Pin_2
#define UARTTXPORT							GPIOA

/**
 * @brief Time period for timers
 */
#define MEASTIMEPERIODINMILISECONDS			1000
#define COMPARISONTIMEPERIODINMILISECONDS	10000

/**
 * @brief Output pins to connect to the fans
 */
#define OUTPIN								GPIO_Pin_6
#define OUTPORT								GPIOC
#define OUT2PIN								GPIO_Pin_7
#define OUT2PORT							GPIOC
#define OUT3PIN								GPIO_Pin_8
#define OUT3PORT							GPIOC	
