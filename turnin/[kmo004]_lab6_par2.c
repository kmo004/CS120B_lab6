
/*	Author: mwen004 (Michael Wen)
 *  Partner(s) Name: Moker(Ke) Bellomo
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

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char button = 0x00;

void TimerOn()
{
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;

  _avr_timer_cntcurr = _avr_timer_M;
  
  SREG |= 0x80;
}

void TimerOff()
{
  TCCR1B = 0x00;
}

void TimerISR()
{
  TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
  _avr_timer_cntcurr--;
  if (_avr_timer_cntcurr == 0)
    {
      TimerISR();
      _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M)
{
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;
}

enum States {Init,SM_1,SM_2,SM_3,HOLD_1,HOLD_2,HOLD_3} state;
void Tick()
{
switch(state)
{
case Init:
state = SM_1;
break;

case SM_1:
if ((~PINA&0x01)==0x01)
{
state = HOLD_1;
}
else
{
state = SM_2;
}
break;

case SM_2:
if ((~PINA&0x01)==0x01)
{
state = HOLD_2;
}
else
{
state = SM_3;
}
break;

case SM_3:
if ((~PINA&0x01)==0x01)
{
state = HOLD_3;
}
else
{
state = SM_1;
}
break;

case HOLD_1:
if ((~PINA & 0x01)==0x01)
{
state = Init;
}
else
{
state = HOLD_1;
}
break;

case HOLD_2:
if ((~PINA & 0x01)==0x01)
{
state = Init;
}
else
{
state = HOLD_2;
}
break;

case HOLD_3:
if ((~PINA & 0x01)==0x01)
{
state = Init;
}
else
{
state = HOLD_3;
}
break;


default: state = Init;

}

switch(state)
{
case SM_1:
PORTB = 0x01;
break;

case SM_2:
PORTB = 0x02;
break;

case SM_3:
PORTB = 0x04;
break;

case HOLD_1:
PORTB = 0x01;
break;

case HOLD_2:
PORTB = 0x02;
break;

case HOLD_3:
PORTB = 0x04;
break;

case Init:
break;
}
}

int main(void) {
    /* Insert DDR and PORT initializations */
  DDRA = 0x00; PORTA = 0xFF;
  DDRB = 0xFF; PORTB = 0x00;
  TimerSet(50);
  TimerOn();

button = (~PINA & 0x01);
state = Init;
    /* Insert your solution below */
    while (1) {
Tick();
      while(!TimerFlag);
      TimerFlag = 0;
    }
}

