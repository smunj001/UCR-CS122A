#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"


//Using temp var for PORT operations
unsigned char temp = 0x00;

enum LEDState {INIT,L0,L1} led_state;


void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		led_state = L0;
		break;
		
		case L0:
		led_state = L1;
		break;
		
		case L1:
		led_state = L0;
		break;
		
		default:
		led_state = L0;
		break;
		
	}
	//Transitions
	switch(led_state){
		case INIT:
		break;
		
		case L0:
		temp = temp | 0x01;
		PORTD = temp;
		break;
		
		case L1:
		temp = temp & 0xFE;
		PORTD = temp;
		break;
		
		default:
		led_state = INIT;
		break;
		
	}
}

//Second

enum LEDState1 {INITone,L0one,L1one} led_state_one;


void LEDS_Init1(){
	led_state_one = INITone;
}

void LEDS_Tick1(){
	//Actions
	switch(led_state_one){
		case INITone:
		led_state_one = L0one;
		break;
		
		case L0one:
		led_state_one = L1one;
		break;
		
		case L1one:
		led_state_one = L0one;
		break;
		
		default:
		led_state_one = L0one;
		break;
		
	}
	//Transitions
	switch(led_state_one){
		case INITone:
		break;
		
		case L0one:
		temp = temp | 0xFB;
		PORTD = temp;
		break;
		
		case L1one:
		temp = temp & 0x04;
		PORTD = temp;
		break;
		
		default:
		led_state_one = INITone;
		break;
		
	}
}

//Third

enum LEDState2 {INITtwo,L0two,L1two} led_state_two;


void LEDS_Init2(){
	led_state_two = INITtwo;
}

void LEDS_Tick2(){
	//Actions
	switch(led_state_two){
		case INITtwo:
		led_state_two = L0two;
		break;
		
		case L0two:
		led_state_two = L1two;
		break;
		
		case L1two:
		led_state_two = L0two;
		break;
		
		default:
		led_state_two = L0two;
		break;
		
	}
	//Transitions
	switch(led_state_two){
		case INITtwo:
		break;
		
		case L0two:
		temp = temp | 0xEF;
		PORTD = temp;
		break;
		
		case L1two:
		temp = temp & 0x10;
		PORTD = temp;
		break;
		
		default:
		led_state_two = INITtwo;
		break;
		
	}
}

//TASK ONE
void LedSecTask()
{
	LEDS_Init();
	for(;;)
	{
		LEDS_Tick();
		vTaskDelay(500);
	}
}


//TASK TWO
void LedSecTask1()
{
	LEDS_Init1();
	for(;;)
	{
		LEDS_Tick1();
		vTaskDelay(1000);
	}
}

//TASK THREE

void LedSecTask2()
{
	LEDS_Init2();
	for(;;)
	{
		LEDS_Tick2();
		vTaskDelay(2500);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LedSecTask, (signed portCHAR *)"LedSecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask1, (signed portCHAR *)"LedSecTask1", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(LedSecTask2, (signed portCHAR *)"LedSecTask2", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}



int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRD = 0xFF;
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}