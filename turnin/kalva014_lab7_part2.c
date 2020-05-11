/*	Author: kennethalvarez
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
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

enum States {Start, Init, ButtonPressed1, OnRelease1, ButtonPressed2, OnRelease2, Reset} state;
unsigned char tmpA;
unsigned char tmpC;

void Tick() {
	switch(state) { //transitions
		case Start:
			state = Init;
			break;
		case Init:
			if((tmpA & 0x01) == 0x01) {
				state = ButtonPressed1;
			}
			else if((tmpA & 0x02) == 0x02) {
				state = ButtonPressed2;
			}
			else {
				state = Init;
			}
				
			if((tmpA & 0x03) == 0x03) { //RESET STATEMENT MIGHT BE WRONG
				state = Reset;
			}
			break;
		case ButtonPressed1:
			state = OnRelease1;

			if((tmpA & 0x03) == 0x03) { //RESET STATEMENT MIGHT BE WRONG
                                state = Reset;
                        }
			break;
		case OnRelease1:
			if((tmpA & 0x01) == 0x01) {
				state = OnRelease1;
			}
			else {
				state = Init;
			}
			break;
		case ButtonPressed2:
			state = OnRelease2;

			if((tmpA & 0x03) == 0x03) { //RESET STATEMENT MIGHT BE WRONG
       				state = Reset;
                        }
			break;
		case OnRelease2:
			if((tmpA & 0x02) == 0x02) { //If button is held down then stay in release
				state = OnRelease2;
			}
			else {
				state = Init;
			}
			break;
		case Reset:
			state = Init;
			break;
		default:
			break; 
	}
	switch(state) { //actions
		case Start:
			break;
		case Init:
			break;
		case ButtonPressed1:
			if(tmpC < 9) {
				//PORTC = PINC +  0x01;
				tmpC = tmpC + 0x01;
			}
			break;
		case OnRelease1:
			break;
		case ButtonPressed2:
			if(tmpC > 0) {
				//PORTC = PINC - 0x01;
				tmpC = tmpC - 0x01;
			}
			break;
		case OnRelease2:
			break;
		case Reset:
			//PORTC = 0x00;
			tmpC = 0x00;
			break;
		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA = 0x00; PORTA = 0xFF;
        DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	TimerSet(100);
	TimerOn();

	state = Start; //Inital state			
	tmpC = 0;
	LCD_init();

    /* Insert your solution below */
    while (1) {
	tmpA = ~PINA;
	Tick();
	//LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_WriteData(tmpC + '0');
	
	while(!TimerFlag);
	TimerFlag = 0;
    }
       return 1;
}
