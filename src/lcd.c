/**
 * @file    lcd.c
 * @brief File defining functions used with HD44780 LCD display
 */
#include "stm32f10x.h"
#include "lcd.h"
#include "globalVar.h"

/**
 * @brief Send one half of the byte
 */
void lcd_sendHalf(uint8_t data)
{
	LCD_E_HIGH;
	GPIO_WriteBit(LCD_D4_PORT, LCD_D4_PIN, (data & 0x01));
	GPIO_WriteBit(LCD_D5_PORT, LCD_D5_PIN, (data & 0x02));
	GPIO_WriteBit(LCD_D6_PORT, LCD_D6_PIN, (data & 0x04));
	GPIO_WriteBit(LCD_D7_PORT, LCD_D7_PIN, (data & 0x08));
	LCD_E_LOW;
}

/**
 * @brief Send byte
 */
void lcd_write_byte(uint8_t data)
{
	lcd_sendHalf(data >> 4);
	lcd_sendHalf(data);

	delay_us(60);

}

/**
 * @brief Send command
 */
void lcd_write_cmd(uint8_t cmd)
{
	LCD_RS_LOW;
	lcd_write_byte(cmd);

}

/**
 * @brief Send one char
 */
void lcd_char(char data)
{
	LCD_RS_HIGH;
	lcd_write_byte(data);

}

/**
 * @brief Change location of the cursor
 */
void lcd_locate(uint8_t x, uint8_t y)
{

	switch(y)
	{
		case 0:
			lcd_write_cmd( LCDC_SET_DDRAM | (LCD_LINE1 + x) );
			break;

		case 1:
			lcd_write_cmd( LCDC_SET_DDRAM | (LCD_LINE2 + x) );
			break;
	}

}

/**
 * @brief Initialize I/O Pins used with display
 */
void lcd_gpio_init( void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = LCD_D4_PIN;
	GPIO_Init(LCD_D4_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LCD_D5_PIN;
	GPIO_Init(LCD_D5_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LCD_D6_PIN;
	GPIO_Init(LCD_D6_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LCD_D7_PIN;
	GPIO_Init(LCD_D7_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
	GPIO_Init(LCD_RS_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = LCD_E_PIN;
	GPIO_Init(LCD_E_PORT, &GPIO_InitStructure);

}

/**
 * @brief Startup function for the display
 */
void LCD_Init(void)
{
	lcd_gpio_init();

	delay_ms(15);

	LCD_E_LOW;
	LCD_RS_LOW;

	lcd_sendHalf(0x03);
	delay_ms(4);
	delay_us(100);
	lcd_sendHalf(0x03);
	delay_us(100);
	lcd_sendHalf(0x03);
	delay_us(100);
	lcd_sendHalf(0x02);
	delay_us(100);

	lcd_write_cmd( LCD_FUNC | LCD_4_BIT | LCDC_TWO_LINE | LCDC_FONT_5x7 );
	lcd_write_cmd( LCD_ONOFF | LCD_DISP_ON );
	lcd_write_cmd( LCD_CLEAR );
	delay_ms(5);
	lcd_write_cmd( LCDC_ENTRY_MODE | LCD_EM_SHIFT_CURSOR | LCD_EM_RIGHT );
}

/**
 * @brief Send char string
 */
void lcd_str(char *text)
{
	while(*text)
		lcd_char(*text++);

}

/**
 * @brief Send char string to specified location on the display
 */
void lcd_str_XY(uint8_t x, uint8_t y, char * text)
{
	lcd_locate(x,y);

	while(*text)
		lcd_char(*text++);

}

/**
 * @brief Make temperature measurement screen
 */
void lcdShowTemperature(void){
	lcd_write_cmd( LCD_CLEAR );
	delay_ms(5);
	lcd_str_XY(0,0,"Temp:");
	lcd_str_XY(9,0,currentTemperatureBuff);
	lcd_locate(13,0);
	lcd_char((char)223);
	lcd_char('C');
	lcd_str_XY(0,1,"Trigger:");
	gcvt(triggerTemperature,3,triggerTemperatureBuff);
	lcd_str_XY(9,1,triggerTemperatureBuff);
	lcd_locate(13,1);
	lcd_char((char)223);
	lcd_char('C');
	lcd=0;
}

/**
 * @brief Make "Hello" screen
 */
void lcdSayHello(void){
	lcd_str_XY(2,0,"!BIENVENIDO!");
	lcd_str_XY(7,1,":)");
	delay_ms(1000);
	lcd=0;
}
