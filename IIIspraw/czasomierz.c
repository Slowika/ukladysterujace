/*
 * czasomierz.c
 *
 * Created: 2015-11-25 17:45:46
 *  Author: Administrator
 */ 
//Autorzy: Agnieszka Słowik i Daniel Misior

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>


void setup()
{
	TCCR0 |= /*CTC*/( 1<<WGM01 ) | ( 0<<WGM00 ) /* /64 */| ( 0<<CS02 ) | ( 1<<CS01 ) | ( 1<<CS00 ) /* set 0C0*/| ( 1<<COM00 ) | ( 1<<COM01 );
}

uint8_t number(uint8_t num) //return screen interpretation of digit
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

uint8_t switch_dot( uint8_t num ) // add/remove dot
{
	return num ^= 128;
}

uint16_t get_part_numb( uint16_t numb, uint16_t part) // get selected part of number
{
	int ret = numb;
	for( int i=0; i<part; ++i)
	{
		ret /= 10;
	}
	return ret % 10;
}

void delay_setup()
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	PORTB = 14;
}
void delay_infinity()
{
	uint16_t counter = 0; // displayed number
	while(1)
	{
		for( int i=0; i<4; ++i) 
		{
			PORTA = number(get_part_numb(counter,i)); // get number interpretation and send it to PORTA
			_delay_ms(60);
			switch_screen(); // go to next screen
		}
		++counter;
	}
	
}

uint16_t global_counter = 0; // current displayed number
uint16_t global_screen = 0; //current selected screen
uint16_t global_interrupt = 1000; //timer
void counter_setup()
{
	delay_setup();
	OCR0 = 250;
	TIMSK |= ( 1<<OCIE0 );
	TCCR0 |= /*CTC*/( 1<<WGM01 ) | ( 0<<WGM00 ) /* /64 */| ( 0<<CS02 ) | ( 1<<CS01 ) | ( 1<<CS00 );
	sei();
}

ISR(TIMER0_COMP_vect)
{
	PORTA = number(get_part_numb(global_counter,global_screen));  // get number interpretation and send it to PORTA
	switch_screen(); //go to next screen
	if( !--global_interrupt ) //if timer alarm
	{
		global_interrupt=1000; //reset timer
		++global_counter; //increase number
	}
}

void switch_screen()
{
		
	PORTB = (~(1 << global_screen)) & 0x0F; // move 1 to screen position, negate and mask
	if( ++global_screen>3 ) global_screen=0;
}

int main(void)
{
	/*delay_setup();
    while(1)
    {
        delay_infinity();
    }*/
	
	counter_setup();
	while(1)
	{
		
	}
}