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
unsigned char Matrix = 0x01;

void ADC_init() 
{
	 ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);	
}

void digitalConversion()
{
	ADCSRA |= ( 1<<ADSC );
	while ( !( ADCSRA & ( 1<<ADIF )));
}

enum SM1_Joystick {init_sm1, right, left} state;
	
void SM1_Joystick_Tick()
{
	digitalConversion();
	input = ADC;
	
	//Transitions
	switch(state)
	{
		case init_sm1:
			if (input > 800)
			{
				state = right;
			}
			else if (input < 200)
			{
				state = left;
			}
			else
			{
				state = init_sm1;
			}
			
			break;
			
		
		case left:
			if (input > 800)
			{
				state = right;	
			}
			else
			{
				state = init_sm1;
			}
			
			break;
		
		case right:
			if (input < 200)
			{
				state = left;
			}
			else
			{
				state = init_sm1;
			}
			
			break;
			
		default:
			break;
	}
	//Actions
	switch(state)
	{
		case init_sm1:
			break;
		
		case left:
			if (Matrix != 0x80)
			{
				Matrix = Matrix << 1;
			}
			else
			{
				Matrix = 0x01;
			}
			break;
		
		case right:
		
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

void SM1_Joystick_Task()
{
	state = init_sm1;
	for(;;)
	{
		SM1_Joystick_Tick();
		vTaskDelay(200);
	}
	
}

enum SM2_Matrix {init_sm2} state_sm2;
	
void SM2_Matrix_Tick()
{
	//Transitions
		
	switch(state_sm2)
	{
		case init_sm2:
			break;
		
		default:
			break;
	}
	
	//Actions
	
	switch(state_sm2)
	{
		case init_sm2:
	
			PORTC = Matrix;
			PORTD = ~0x01;
			break;
		
		default:
			break;
	}
}

void SM2_Matrix_Task()
{
	state = init_sm2;
	for(;;)
	{
		SM2_Matrix_Tick();
		vTaskDelay(200);
	}
	
}


void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(SM1_Joystick_Task, (signed portCHAR *) "SM1_Joystick_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(SM2_Matrix_Task, (signed portCHAR *) "SM2_Matrix_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}

int main(void)
{
	DDRC = 0xFF; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;

	ADC_init();
	StartShiftPulse(1);
	vTaskStartScheduler();
	
	
	return 0;
}