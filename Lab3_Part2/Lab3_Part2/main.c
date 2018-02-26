/*
 * Lab3_Part2.c
 *
 * Created: 10/19/2017 6:16:23 PM
 * Author : Karthik Munjeti
 */ 

#include <avr/io.h>

unsigned char receivedData;
unsigned char pattern;
unsigned int speed_set;
unsigned char speed;

//START TIMER
volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

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
Timer_ISR(TIMER1_COMPA_vect) {
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

//END TIMER

void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	
	DDRB = 0x40;
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	
	//SPCR = SPCR|0xC0;
	SPCR |= (1<<SPE) | (1<<SPIE);
	
	// make sure global interrupts are enabled on SREG register (pg. 9)
	
	//SREG = SREG|0x80;
}

ISR(SPI_STC_vect) { // this is enabled in with the SPCR register’s “SPI
	// Interrupt Enable”
	// SPDR contains the received data, e.g. unsigned char receivedData =
	// SPDR;
	
	receivedData = SPDR;
}



enum sm_1  {wait, s0, s1 } sm1_state;
enum sm_2 {sm2_wait, sm2_s0, sm2_s1} sm2_state;
//enum sm_3 {sm3_wait, s0, s1, s2, s3, s4, s5, s6 } sm3_state;
//enum sm_4 {wait, s0, s1, s2, s3, s4, s5, s6 } sm4_state;

void SMP1()
{
	
	//Transitions
	switch(sm1_state)
	{
		case wait:
			sm1_state = s0;
			
		case s0:
			sm1_state = s1;
		
		case s1:
			sm1_state = s0;
		
		default:
			break;
	}
	
	//Actions
	switch(sm1_state)
	{
		case wait:
			break;
		
		case s0:
			PORTA = 0xF0;
			break;
		
		case s1:
			PORTA = 0x0F;
			break;
			
		default:
			break;
	}
	
	
}

void SMP2()
{
	//Transitions
	switch(sm2_state)
	{
		case sm2_wait:
			sm2_state = sm2_s0;
		
		case sm2_s0:
			sm2_state = sm2_s1;
		
		case sm2_s1:
			sm2_state = sm2_s0;
		
		default:
			break;
	}
	
	//Actions
	switch(sm2_state)
	{
		case sm2_wait:
			break;
					
		case sm2_s0:
			PORTA = 0XAA;		
			
		case sm2_s1:
			PORTA = 0x55;	
				
		default:
			break;
	}
	
	
}
/*
void SMP3()
{
	//Transitions
	switch(sm3_state)
	{
		
	}
	
	//Actions
	switch(sm3_state)
	{
		
	}	
}

void SMP4()
{
	//Transitions
	switch(sm4_state)
	{
		
	}
	
	//Actions
	switch(sm4_state)
	{
		
	}	
	
}

*/

void getPattern()
{
	pattern = (receivedData & 0xF0); //4 Possible Patterns
	speed = (receivedData & 0x0F);  //6 Possible Speeds
	
	//Check the pattern chosen
	if(pattern == 0x10)  //A
	{
		pattern = 0xF0;
	}
	
	if(pattern == 0x20)  //B
	{
		pattern = 0xAA;
	}
	
	if(pattern == 0x40)  //C
	{
		pattern = 0x80;
	}
	
	if(pattern == 0x80)  //D
	{
		pattern = 0x55;
	}
	
	
	//Check Speed Chosen
	
	if(speed == 0x01)  //1
	{
		speed_set = 2000;
	}
	
	if(speed == 0x02)  //2
	{
		speed_set = 1000;
	}
	
	if(speed == 0x03)  //3
	{
		speed_set = 500;
	}
	
	if(speed == 0x04)  //4
	{
		speed_set = 250;
	}
	
	if(speed == 0x05)  //5
	{
		speed_set = 100;
	}
	
	if(speed == 0x06)  //6
	{
		speed_set = 50;
	}
	
}
int main(void)
{
	
		DDRA = 0xFF;	PORTA = 0x00;
		DDRB = 0x00;	PORTB = 0xFF;
		
		TimerOn();
		
		SPI_ServantInit();
		
	
    /* Replace with your application code */
    while (1) 
    {
		getPattern();
		TimerSet(speed_set);
		if(pattern == 0xF0)
		{
			SMP1();
		}
		else if(pattern == 0xAA)
		{
			SMP2();
		}
		/*
		else if(pattern == 0x80)
		{
			SMP3();
		}
		else if(pattern == 0x55)
		{
			SMP4();
		}
		*/
		//else if(pattern == 0x80)
    }
}

