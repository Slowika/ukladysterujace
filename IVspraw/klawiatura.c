/*
 * klawiatura.c
 *
 * Created: 2015-12-02 17:54:28
 *  Autorzy: Agnieszka Słowik, Daniel Misior
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define SCREEN_PORT PORTB
#define SCREEN_DDR DDRB
#define KEYBOARD_PORT PORTC
#define KEYBOARD_DDR DDRC
#define KEYBOARD_OUTPUT PINC
#define OUTPUT_PORT PORTA
#define OUTPUT_DDR DDRA

uint16_t global_counter = 0;
uint16_t global_screen = 0;
uint16_t global_interrupt = 50;
uint8_t number(uint8_t num)
{
	switch(num)
	{
		case 0: return ~0b00111111;
		case 1: return ~0b00000110;
		case 2: return ~0b01011011;
		case 3: return ~0b01001111;
		case 4: return ~0b01100110;
		case 5: return ~0b01101101;
		case 6: return ~0b01111101;
		case 7: return ~0b00000111;
		case 8: return ~0b01111111;
		case 9: return ~0b01101111;
		default: return 0xFF;
	}
}
uint8_t switch_dot( uint8_t num )
{
	return num ^= 128;
}
uint16_t get_part_numb( uint16_t numb, uint16_t part)
{
	int ret = numb;
	for( int i=0; i<part; ++i)
	{
		ret /= 10;
	}
	return ret % 10;
}
void switch_screen()
{
	
	SCREEN_PORT = (~(1 << global_screen)) & 0x0F;
	if( ++global_screen>3 ) global_screen=0;
}
uint8_t get_key_pressed(uint8_t x, uint8_t y)
{
	if( !x || !y ) return 0;
	uint8_t row,column;
	switch(x)
	{
		case 8: column=1; break;
		case 4: column=2; break;
		case 2: column=3; break;
		case 1: column=4; break;
	}
	switch(y)
	{
		case 128: row=1; break;
		case 64: row=2; break;
		case 32: row=3; break;
		case 16: row=4; break;
	}
	return (row-1)*4+column;
}

void counter_setup()
{
	OUTPUT_DDR = 0xFF;
	SCREEN_DDR = 0xFF;
	SCREEN_PORT = 14;
	OCR0 = 250;
	TIMSK |= ( 1<<OCIE0 );
	TCCR0 |= /*CTC*/( 1<<WGM01 ) | ( 0<<WGM00 ) /* /64 */| ( 0<<CS02 ) | ( 1<<CS01 ) | ( 1<<CS00 );
	sei();
}

ISR(TIMER0_COMP_vect)
{
	OUTPUT_PORT = number(get_part_numb(global_counter,global_screen));
	switch_screen();
	if( !--global_interrupt )
	{
		global_interrupt=50;
		uint8_t x,y;
		//read column
		KEYBOARD_DDR = 0b11110000;
		KEYBOARD_PORT = 0b00001111;
		for(uint8_t i=20; i>0; --i); //delay
		x = ~KEYBOARD_OUTPUT & 0b00001111;
		
		//read row
		KEYBOARD_DDR = ~0b11110000;
		KEYBOARD_PORT = ~0b00001111;
		for(uint8_t i=20; i>0; --i); //delay
		y = ~KEYBOARD_OUTPUT & ~0b00001111;
		global_counter = get_key_pressed(x,y);
	}
	
}

int main(void)
{
	counter_setup();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}