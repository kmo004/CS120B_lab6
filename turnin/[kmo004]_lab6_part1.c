/*	Author: kmo004
 *  Partner(s) Name: Michael Wen
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum states {LED1,LED2,LED3} state;
volatile unsigned char TimerFlag = 0;

unsigned char cnt = 0x01;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerON(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void Tick(){
	switch(state){
		
		case LED1:
		state = LED2;
		break;
		
		case LED2:
		state = LED3;
		break;
		
		case LED3:
		state = LED1;
		break;

	}
	switch(state){
		
		case LED1:
			PORTB = 0x01;
		break;
		
		case LED2:
			PORTB = 0x02;
		break;
		
		case LED3:
			PORTB = 0x04;
			
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(150);
	TimerON();

    /* Insert your solution below */
    state = LED1;

    while (1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0x00;
		
		
    }
    return 1;
}
