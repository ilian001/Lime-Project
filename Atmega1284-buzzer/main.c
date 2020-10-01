/*
 * project test.c
 *
 * Created: 11/15/2019 2:06:33 PM
 * Author : user
 */ 

#include <avr/io.h>
#include "timer.h"


unsigned char input = 0x00;
unsigned char count = 0x00;
double freqs[] = {987.77,783.99,659.25,493.88,392,329.63,261.63};
//987.77,783.99,659.25,493.88,392,329.63,261.63

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0) | (1 << WGM00);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}

enum StatesBuzz {init,b1,b2,b3,b4,b5,b6,b7,turnoff} stateBuzz;
void BuzzOn() {
	switch(stateBuzz) {			//transition
		case init:
			PWM_off();
			if(input) {
				PWM_on();
				stateBuzz = b1;
			}
		break;
		case b1:
			stateBuzz = b2;
		break;
		case b2:
			stateBuzz = b3;
		break;
		case b3:
			stateBuzz = b4;
		break;	
		case b4:
			stateBuzz = b5;
		break;	
		case b5:
			stateBuzz = b6;
		break;
		case b6:
			stateBuzz = b7;
		break;
		case b7:
			if(!input) {
				stateBuzz = turnoff;
				count = 0;
				PWM_on();
			}
		break;
		case turnoff:
		if(count > 7) {
			stateBuzz = init;
		}
		break;
	}
	switch(stateBuzz) {			//action
		case init:
		break;
		case b1:
			set_PWM(261.63);	
		break;
		case b2:
			set_PWM(329.63);
		break;
		case b3:
			set_PWM(392);
		break;
		case b4:
			set_PWM(493.88);
		break;
		case b5:
			set_PWM(659.25);
		break;
		case b6:
			set_PWM(783.99);
		break;
		case b7:
			set_PWM(987.77);
			PWM_off();
		break;
		case turnoff:
			count++;
			set_PWM(freqs[count]);
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	
	DDRB = 0xFF; PORTB = 0x00;
	PWM_on();
	set_PWM(1);
	TimerSet(100);
	TimerOn();
	
	stateBuzz = init;
	
    while (1) 
    {
		input = ~PINA;
		BuzzOn();	
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

