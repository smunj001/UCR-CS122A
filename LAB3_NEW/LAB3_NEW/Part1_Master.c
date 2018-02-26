/*
 * CS122A Lab 3.c
 *
 * Created: 10/17/2017 12:35:01 AM
 * Author : User
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "bit.h"
#include "keypad.h"
#include "lcd.h"


volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
unsigned char pattern = 0x00;
unsigned char speed = 0x00;
unsigned char data = 0x00;
unsigned char pb;
unsigned char x;
void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;    // Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG = 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

//--------Task scheduler data structure--------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;
//--------End Task scheduler data structure----------------


void SPI_MasterInit(void){
	//set DDRB to have miso line as input and everything else as output
	DDRB = 0xBF; //1011 1111
	//enable SPI, Master, set clock rate fck/16
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	//enable global interrupt Enable
	SREG = SREG|0x80;
}

void SPI_MasterTransmit(unsigned char cData){
	//start transmission
	SPDR = cData;
	pb = SetBit(pb, 4, 0);
	PORTB = pb;
	//wait for transmission complete
	while(!(SPSR & (1<<SPIF))){
	;
	}
	pb = SetBit(pb, 4, 1);
	PORTB = pb;
	
}

enum States1{init1, getpat} state1;
int tick_pattern(int state1){
				
	switch(state1){//transitions
	case init1:
	state1 = getpat;
				//		LCD_DisplayString(17,(const unsigned char*) "1");
	break;
	case getpat:
	break;
	default:
	state1 = init1;
	break;
	}//transitions
		
	switch(state1){//actions
	case init1:
				//			LCD_DisplayString(17,(const unsigned char*) "2");
	break;
	case getpat:
	//LCD_DisplayString(17,(const unsigned char*) "3");
	 pattern = GetKeypadKey();

	 
	break;
	default:
	break;
	
	}//actions
	
	return state1;
}

enum States2{init2, getspeed} state2;
int tick_speed(int state2){
		switch(state2){//transitions
		case init2:
		state2 = getspeed;
		break;
		case getspeed:
		break;
		default:
		state2 = init2;
		break;
			
		}//transitions

		switch(state2){//actions
		case init2:
		break;
		case getspeed:
	//	LCD_DisplayString(17,(const unsigned char*) "4");
		speed = GetKeypadKey();

		break;
		default:
		break;
			
		}//actions
	return state2;
}

		unsigned char pt;
		unsigned char st;
enum States3{init3, transmit } state3;
int tick_transmit(int state3){

		switch(state3){//transitions
		case init3:
		state3 = transmit;
		break;
		case transmit:
		break;
		default:
		state3 = init3;
		break;
			
		}//transitions
		
		switch(state3){//actions
		case init3:
		pt = 0x10;
		st = 0x01;
		data = 0x11;
		break;
		case transmit:
					//			LCD_DisplayString(17,(const unsigned char*) "5");
			switch(pattern)
			{
			case 'A': pt = 0x10; break;
			case 'B': pt = 0x20; break;
			case 'C': pt = 0x30; break;
			case 'D': pt = 0x40; break;
			default: break;
			} 
			switch(speed)
			{
			case '1': st = 0x01; break;
			case '2': st = 0x02; break;
			case '3': st = 0x03; break;
			case '4': st = 0x04; break;
			case '5': st = 0x05; break;
			case '6': st = 0x06; break;
			default: break;
			}
			data = pt | st;
			
		break;
		default:
		break;
		}//actions
	return state3;
}
enum States4{init4, displaypt, displaysp} state4;
	int tick_LCD(int state4){
		switch(state4)
		{//transitions
		case init4:
		state4 = displaypt;
		break;
		case displaypt:
		//state4 = displaysp;
		break;
// 		case displaysp:
// 		state4 = displaypt;
// 		break;
		default:
		state4 = init4;
		break;
		}//transitions
		
		switch(state4)
		{//actions
			case init4:	
			LCD_DisplayString(1,(const unsigned char*)" Ptrn:");	
			LCD_DisplayString(9,(const unsigned char*) " Spd:");
			LCD_DisplayString(17, (const unsigned char*) " uC: 1");
			break;
			case displaypt:
			//PATTERN
			//LCD_Cursor(7);
				//		LCD_DisplayString(17,(const unsigned char*) "6");
			if(pt == 0x10)
			LCD_DisplayString( 7,(const unsigned char*) "1");
			else if(pt == 0x20)
			LCD_DisplayString( 7, (const unsigned char*)"2");
			else if(pt == 0x30)
			LCD_DisplayString(7, (const unsigned char*)"3");
			else if(pt == 0x40)
			LCD_DisplayString( 7, (const unsigned char*)"4");
			
	//		break;
	//		case displaysp:
			//SPEED
					//				LCD_DisplayString(17,(const unsigned char*) "7");
			//LCD_Cursor(17);
			if(st ==0x01)
			LCD_DisplayString(14, (const unsigned char*)"1");
			else if(st == 0x02)
			LCD_DisplayString(14, (const unsigned char*)"2");
			else if(st== 0x03)
			LCD_DisplayString(14, (const unsigned char*)"3");
			else if(st == 0x04)
			LCD_DisplayString(14, (const unsigned char*)"4");
			else if(st == 0x05)
			LCD_DisplayString(14, (const unsigned char*)"5");
			else if(st== 0x06)
			LCD_DisplayString(14, (const unsigned char*)"6");
			break;
			default:
			break;
		}//actions
		return state4;
		}

int main(void)
{
    /* Replace with your application code */

	//DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, ouutputs init 0's
	DDRA = 0xF0; PORTA = 0x0F; //PC7...4 outputs init 0s, PC3...0 inputs init 1s
	DDRC = 0xFF; PORTC = 0x00; //LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	pb = 0xBF;
	
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(7);
	
	//GCD of all tasks
	unsigned long int GCD = 50;
	//GCD periods for scheduler
	unsigned long int SMTick1_period = 50;//replace SMTick1_period with state machine you're using
	unsigned long int SMTick2_period = 50;
	unsigned long int SMTick3_period = 50;
	unsigned long int SMTick4_period = 50;
	
	//Declare an array of tasks
	static task task1, task2, task3, task4;
	task *tasks[] = { &task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//task 1 -- ONLY TASK
	task1.state = -1;
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &tick_pattern;
	//task 2
	task2.state = -1;
	task2.period = SMTick2_period;
	task2.elapsedTime = SMTick2_period;
	task2.TickFct = &tick_speed;
	//task3
	task3.state = -1;
	task3.period = SMTick3_period;
	task3.elapsedTime = SMTick3_period;
	task3.TickFct = &tick_transmit;	
	//task4
	task4.state = -1;
	task4.period = SMTick4_period;
	task4.elapsedTime = SMTick4_period;
	task4.TickFct = &tick_LCD;
	
	//Set the timer and turn it on
	TimerSet (GCD);
	TimerOn();
	//unsigned char total = 0;
	//LCD_WriteData(total + '0');
	SPI_MasterInit();
	unsigned short i;
    while(1) 
    {	
	//x = GetKeypadKey();
			// Scheduler code
	//LCD_ClearScreen();
			for ( i = 0; i < numTasks; i++ ) {
				// Task is ready to tick
				if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
	
				}
					//LCD_DisplayString(1, "Ptrn: .");
				tasks[i]->elapsedTime += 1;
			}
						
			SPI_MasterTransmit(data);

   
	while (!TimerFlag); //Wait 1 sec
	TimerFlag = 0;
	
	}
	
	}