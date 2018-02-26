/*
 * LAB3_Slave.c
 *
 * Created: 10/19/2017 5:44:21 PM
 * Author : Karthik Munjeti
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "bit.h"

unsigned char receivedData = 0x00;

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
enum servantSM {servant_wait, servant_read} states;void TickFct_servant(){	switch(states)	{		case servant_wait:		states = servant_read;		break;				case servant_read:		states = servant_wait;		break;				default:		states = servant_wait;		break;	}		switch(states)	{		case servant_wait:		break;				case servant_read:		PORTA = receivedData;		break;				default:		break;	}}
int main(void)
{	
	TimerSet(50);
	TimerOn();
	
	while(1)
	{
		
		SPI_ServantInit();
		TickFct_servant();
	}
	
}

