/*
 * GccApplication3.c
 *
 * Created: 2015-11-04 18:09:57
 *  Author: Administrator
 */ 
#include <stdbool.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <avr/delay.h>
////////////////////////////////////////////////////
void po_prostu_swiec()
{
	DDRA = 0xFF;
	PORTA = 0xFF;
}
/////////////////////////////////// snake w obie strony
uint16_t snake_counter = 0;
bool snake_flag = true;
void snake()
{
	if(snake_counter==0)
	{
		snake_counter=1;
		snake_flag=true;
	}
	else if( snake_counter==256 )
	{
		snake_flag = false;
		snake_counter/=2;
	}
	PORTA = snake_counter;
	snake_counter = snake_flag?snake_counter*2:snake_counter/2;
	
	_delay_ms(1000);
}
/////////////////////////////////////// gasnie i zapala sie na przycisk
bool shine_flag = true;
uint8_t shine_counter = 0;
void shine()
{
	PORTA = 0xFF;
	_delay_ms(shine_counter);
	PORTA = 0;
	_delay_ms(100-shine_counter);
	if( shine_flag ) ++shine_counter;
	else if(shine_counter)--shine_counter;
	if( shine_counter>100 )
	{
		shine_counter=100;
	}

	uint8_t count = 100;
	while( count-->0)
	{
		if( !(PINC & 0x80) ) 
		{
			if( shine_flag && shine_counter==100 )
			{
				shine_flag=false;
				--shine_counter;
			}
			else if( !shine_flag && shine_counter==0)
			{
				
				shine_flag = true;
				++shine_counter;
			}
		}
		_delay_ms(1);
	}
}
///////////////////////////////////////////////// zadanie dodatkowe
uint8_t pow( uint8_t ind )
{
	uint8_t out = 1;
	while(ind-->0)
		out*=2;
	return out;
}

void fire_snake()
{
	uint8_t pointer =0;
	uint8_t tab[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t count =1;
	uint8_t second_count = 0;
	uint8_t flag = 1;
	while(1)
	{
		count = 1;
		uint8_t out_numb = 0;
		while( count++<=100 )
		{
			for( uint8_t i=7; i>=0; --i)
			{
				if( count<=tab[i] )
					out_numb += pow(7-i);
			}
			PORTA = out_numb;
			_delay_ms(1);
		}
		_delay_ms(100);
		for( uint8_t i=0; i<7; ++i) --tab[i];
		if( ++second_count==10 )
		{
			second_count = 0;
			tab[pointer]=100;
			pointer -= flag;
			if( pointer==7 || pointer==0 ) flag *= -1;
		}
	}	
}


int main(void)
{
	DDRA = 0xFF;
	DDRC = 0;
	PORTC = 0b10000000;
    while(1)
    {
		//po_prostu_swiec()
		//snake();
		//shine();

		fire_snake();
    }
}