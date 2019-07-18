/**
 * @file    globalVar.h
 * @brief File containing global variables
 * and flags
 */

/**
 * @brief Default trigger temperature used if
 * value cannot be found elsewhere
 */
#define DEFAULTTRIGGER 26.50

/**
 * @brief temperature values
 */
float triggerTemperature;
float currentTemperature;

/**
 * @brief LCD buffers
 */
char currentTemperatureBuff[3];
char triggerTemperatureBuff[3];
/**
 * @brief Flag - temperature is being changed
 */
int changingTemperature;
/**
 * @brief Flag - refresh the lcd screen
 */
int lcd;
