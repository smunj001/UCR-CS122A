/*
 * LabElectiveJoyStick.c
 *
 * Created: 10/27/2017 1:10:55 AM
 * Author : Karthik Munjeti
 */ 


#include <avr/io.h>

unsigned short input;

void ADC_init() 
{
	
	 ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);	
}

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char C = 0x00;
	unsigned char D = 0x00;
	
	ADC_init();
	
	while(1)
	{
			input = ADC;
			C = (char) input;
			D = (char) (input >> 8);
			
			PORTD= C;
			PORTC = D;
		
	}
	
	
	return 0;
}