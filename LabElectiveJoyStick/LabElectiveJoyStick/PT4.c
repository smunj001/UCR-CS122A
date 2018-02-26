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
unsigned int setSpeed = 1000;

void ADC_init() 
{
	 ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);	
}

// Pins on PORTA are used as input for A2D conversion
//    The default channel is 0 (PA0)
// The value of pinNum determines the pin on PORTA
//    used for A2D conversion
// Valid values range between 0 and 7, where the value
//    represents the desired pin for A2D conversion
void Set_A2D_Pin(unsigned char pinNum) {
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	// Allow channel to stabilize
	static unsigned char i = 0;
	for ( i=0; i<15; i++ ) { asm("nop"); }
}


void digitalConversion()
{
	ADCSRA |= ( 1<<ADSC );
	while ( !( ADCSRA & ( 1<<ADIF )));
}

enum SM1_Joystick {init_sm1, right, left, up, down} state;
	
void SM1_Joystick_Tick()
{
	input = ADC;
	//The illuminated LED should shift once every 1000ms, 500ms, 250ms, or 100ms
	
	//Transitions
	switch(state)
	{
		case init_sm1:
			if (input > 800)
			{
				state = right;
				setSpeed = 100;
			}
			else if(input > 700 && input < 799 )
			{
				state = right;
				setSpeed = 250;
			}
			else if(input > 650 && input < 699 )
			{
				state = right;
				setSpeed = 500;
			}
			else if(input > 500 && input < 649 )
			{
				state = right;
				setSpeed = 1000;
			}
			else if (input < 200)
			{
				state = left;
				setSpeed = 100;
			}
			else if (input < 300 && input > 201)
			{
				state = left;
				setSpeed = 250;
			}
			else if (input < 400 && input > 301)
			{
				state = left;
				setSpeed = 500;
			}
			else if (input < 499 && input > 401)
			{
				state = left;
				setSpeed = 1000;
			}
			else
			{
				state = init_sm1;
			}
			
			break;
			
		
		case left:
			Set_A2D_Pin(0x00);
			digitalConversion();

			if (input > 800)
			{
				state = right;
				setSpeed = 100;
			}
			else if(input > 700 && input < 799 )
			{
				state = right;
				setSpeed = 250;
			}
			else if(input > 650 && input < 699 )
			{
				state = right;
				setSpeed = 500;
			}
			else if(input > 500 && input < 649 )
			{
				state = right;
				setSpeed = 1000;
			}
			else
			{
				state = init_sm1;
			}
			
			break;
		
		case right:
			Set_A2D_Pin(0x00);
			digitalConversion();
			
			if (input < 200)
			{
				state = left;
				setSpeed = 100;
			}			
			else if (input < 300 && input > 201)
			{
				state = left;
				setSpeed = 250;
			}
			else if (input < 400 && input > 301)
			{
				state = left;
				setSpeed = 500;
			}
			else if (input < 499 && input > 401)
			{
				state = left;
				setSpeed = 1000;
			}
			else
			{
				state = init_sm1;
			}
			
			break;
			
		case up:
			Set_A2D_Pin(0x00);
			digitalConversion();

			if (input > 800)
			{
				state = down;
				setSpeed = 100;
			}
			else if(input > 700 && input < 799 )
			{
				state = down;
				setSpeed = 250;
			}
			else if(input > 650 && input < 699 )
			{
				state = down;
				setSpeed = 500;
			}
			else if(input > 500 && input < 649 )
			{
				state = down;
				setSpeed = 1000;
			}
			else
			{
				state = init_sm1;
			}
			
			break;
			
		case down:
			Set_A2D_Pin(0x00);
			digitalConversion();
			
			if (input < 200)
			{
				state = up;
				setSpeed = 100;
			}
			else if (input < 300 && input > 201)
			{
				state = up;
				setSpeed = 250;
			}
			else if (input < 400 && input > 301)
			{
				state = up;
				setSpeed = 500;
			}
			else if (input < 499 && input > 401)
			{
				state = up;
				setSpeed = 1000;
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
			setSpeed = 1000;
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
			
			case up:
			if (Matrix != 0x80)
			{
				Matrix = Matrix << 1;
			}
			else
			{
				Matrix = 0x01;
			}
			break;
			
			case down:
			
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