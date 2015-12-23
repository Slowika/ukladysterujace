/*
 * GccApplication1.c
 *
 * Created: 2015-11-18 17:58:24
 *  Author: Administrator
 */ 

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

///////////////////////////////////////////
bool canClick=1; // sprawdza czy zaliczyc klikniecie
int8_t counter = 0;
void Blink() // funkcja odpowiadajaca za miganie jednej diody
{
	PORTA ^= 128; //odwrocenie ostatniego bitu
	canClick = 1; // mozna kilkac raz na sekunde
	_delay_ms(1000);
}

void Setup() //ustawienia
{
	DDRA = 0xFF;
	DDRD = 0;
	PORTD |= ( 1<< PD2 );
	MCUCR |= ( 1<< ISC01 ) | ( 0<<ISC00 );
	GICR |= ( 1<< INT0 );
	sei();
}

ISR(INT0_vect) //przerwanie przy kliknieciu
{
	if( canClick ) //jesli mozna kliknac
	{
		++counter; //zwieksz licznik
		if( counter==128 ) counter =0; //ostatnia didoa jest do migania wiec musismy ograniczyc licznik do 128
		canClick=0; //juz nie mozna klikac
		PORTA = counter;
	}
}
///////////////////////////////////////////////////////////////
void SetupLightManager() //ustawienia
{
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0;
	PORTC = ( 1<<PC0 ) | ( 1<<PC1 );
	
	OCR0 = 0;
	TCCR0 |= ( 1<< WGM01 ) | ( 1<<WGM00) | ( 1<<CS02 ) | ( 0<<CS01 ) | ( 1<<CS00 ) | ( 1<<COM00 ) | ( 1<<COM01 );
}
void LightManager() // przy kliknieciu zmienia wartosc porownywana z licznikiem, po odpowiednim podpieciu fizycznym daje prosty
		    // system kontroli jasnosci
{
	
	if( !(PINC & 0x01) )
	{
		++OCR0; //po kliknieciu zwieksz liczbe porownywana z licznikiem
	}
	else if( !(PINC & 0x02) )
	{
		
		--OCR0; //po kliknieciu zmniejsz liczbe porownywana z licznikiem
	}
	_delay_ms(100);
}
                                                  
//////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	/*Setup();
    while(1)
    {
		Blink();    
    }*/
	SetupLightManager();
	while(1)
	{
		LightManager();
		
	}
}