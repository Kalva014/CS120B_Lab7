/*	Author: kennethalvarez
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "io.h"
#include "timer.h"

enum States {Start, Init, Led2, Led3, ButtonPressed, ButtonReleased, ButtonPressed2} state;
unsigned char tmpA;
unsigned char tmpB;
unsigned char prevState;
unsigned char score;

void Tick() {
	switch(state) { //transitions
		case Start:
			state = Init;
			break;
		case Init:
			prevState = Init;
			state = Led2;
			
			if((tmpA & 0x01) == 0x01) {
				prevState = Init;
				state = ButtonPressed;
			}
			break;
		case Led2:
			if(prevState == Init) {
				state = Led3;
			}
			else {
				state = Init;	
			}

			if((tmpA & 0x01) == 0x01) {
				prevState = Led2;
				state = ButtonPressed;
			}
			break;
		case Led3:
			prevState = Led3;
			state = Led2;
			
			if((tmpA & 0x01) == 0x01) {
				prevState = Led3;
				state = ButtonPressed;
			}
			break;
		case ButtonPressed:
			if((tmpA & 0x01) == 0x01) {
				state = ButtonPressed;
			}
			else {
				state = ButtonReleased;
			}
			
			if(prevState == Led2) { //new part
				score = score + 1;
			}
			else {
				if(score > 0) {
					score = score - 1;
				}
			}
			break;
		case ButtonReleased:
			if((tmpA & 0x01) == 0x01) {
				state = ButtonPressed2;
			}
			else {
				state = ButtonReleased;
			}
			break;
		case ButtonPressed2:
			state = Init;
			break;
		default:
			break;
	}
	
	switch(state) { //actions
		case Start:
			break;
		case Init:
			tmpB = 0x01;
			break;
		case Led2:
			tmpB = 0x02;
			break;
		case Led3:
			tmpB = 0x04;
			break;
		ButtonPressed: 		
			break;
		ButtonReleased:
			break;
		ButtonPressed2:
			break;	
		default:
			break;		
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x0F; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	state = Start;
	prevState = Init;
	score = 5;	

	TimerSet(300);
	TimerOn();
	
        LCD_init();	

    /* Insert your solution below */
    while (1) {
	tmpA = ~PINA;
	tmpB = PINB;
	Tick();
	PORTB = tmpB;
	
	LCD_Cursor(1);	
	if(score < 0){
		LCD_WriteData('0');
	}
	else if(score == 9) {
		LCD_DisplayString(1, "Winner!");
	}
	else {
		LCD_WriteData(score + '0');	
	}
	
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
