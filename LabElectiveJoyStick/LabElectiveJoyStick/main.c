/*
 * LabElectiveJoyStick.c
 *
 * Created: 10/27/2017 1:10:55 AM
 * Author : Karthik Munjeti
 */ 


#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

unsigned short input; //Joystick Input
unsigned char Matrix;
enum SM1{init, Left, Right} state;

void ADC_init() 
{
	 ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);	
}

void digitalConversion()
{
	ADCSRA |= ( 1<<ADSC );
	while ( !( ADCSRA & ( 1<<ADIF )));
}

	
void SM1_Tick()
{
	digitalConversion();
	input = ADC;
	
	//Transitions
	switch(state)
	{
		case init:
			if (input > 800)
			{
				state = Right;
			}
			else if (input < 200)
			{
				state = Left;
			}
			else
			{
				state = init;
			}
			
			break;
			
		
		case Left:
			if (input > 800)
			{
				state = Right;	
			}
			else
			{
				state = init;
			}
			
			break;
		
		case Right:
			if (input < 200)
			{
				state = Left;
			}
			else
			{
				state = init;
			}
			
			break;
			
		default:
			break;
	}
	//Actions
	switch(state)
	{
		case init:
			break;
		
		case Left:
			if (Matrix != 0x80)
			{
				Matrix = Matrix << 1;
			}
			else
			{
				Matrix = 0x01;
			}
		
		case Right:
		
			if (Matrix != 0x01)
			{
				Matrix = Matrix >> 1;
			}
			else
			{
				Matrix = 0x80;
			}
			break;
			
		default:
			break;	
	}
}

void SM1_Task()
{
	state = init;
	for(;;)
	{
		SM1_Tick();
		vTaskDelay(200);
	}
	
}



int main(void)
{
	DDRC = 0xFF; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;

	ADC_init();
	
	
	return 0;
}