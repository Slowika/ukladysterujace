/*
 * display.c
 *
 * Created: 2015-12-09 18:06:08
 *  Author: Administrator
 */ 

#include "HD44780.h"
#define DISPLAY_DDR DDRB
#define DISPLAY_PORT PORTB
#define PIN_E PB4
#define PIN_RS PB5

void WriteNibble(uint8_t nibbleToWrite)
{
	//E=1
	DISPLAY_PORT |= (1<<PIN_E);
	
	//write
	DISPLAY_PORT = (DISPLAY_PORT & 0xF0) | (nibbleToWrite & 0x0F);
	
	//E=0
	DISPLAY_PORT &= ~(1<<PIN_E);
}


void WriteByte(uint8_t dataToWrite)
{
	WriteNibble(dataToWrite>>4);
	WriteNibble(dataToWrite);
}


void LCD_Command(uint8_t cmd)
{
	DISPLAY_PORT &= ~(1<<PIN_RS);
	WriteByte(cmd);
};

void LCD_Text(char * text)
{
	DISPLAY_PORT |= (1<<PIN_RS);
	while( *text ) //while not 0
	{
		WriteByte(*(text++)); //write value
		_delay_us(50);
	}
};
void LCD_GoToXY(uint8_t x, uint8_t y)
{
	uint8_t pos = y*0x40 + x;
	LCD_Command(0b10000000 | (pos & 0b011111111));
	_delay_ms(1);
};

void LCD_Clear(void)
{
	LCD_Command(0b00000001);
	_delay_ms(3);
};

void LCD_Home(void)
{
	LCD_Command(0b00000010);
	_delay_ms(2);
};

void LCD_Initalize(void) //initialization chain
{
	_delay_ms(50);
	for( uint8_t i=3; i--;)
	{
		WriteNibble(0b00000011);
		_delay_ms(5);
	}
	WriteNibble(0b00000010);
	_delay_ms(10);
	WriteByte(0b00101000);
	_delay_ms(10);
	WriteByte(0b00001000);
	_delay_ms(10);
	WriteByte(0b0000001);
	_delay_ms(10);
	WriteByte(0b00000110);
	_delay_ms(2);
	LCD_Command(0x0F);
	_delay_ms(2);
	LCD_Command(0x80);
	_delay_ms(2);
};

void LCD_MoveLeft() //shift display to the left
{
	LCD_Command(0b00011000);
	_delay_ms(2);
}
void LCD_MoveRight() //shif display to the right
{
	LCD_Command(0b00011100);
	_delay_ms(2);
}

int main()
{
	DISPLAY_DDR = 0xFF;
	LCD_Initalize();
	for(uint8_t i=0; i<4; ++i)
	{
		LCD_GoToXY(1+(4*i),0);
		LCD_Text("|");
		LCD_GoToXY((4*i),1);
		char* text = {'0','\0'}; // not working, taking first element as an address
		char* text2 = "0";
		LCD_Text(text2);
	}
	while(1)
	{
		LCD_MoveLeft();
		_delay_ms(100);
	}
}