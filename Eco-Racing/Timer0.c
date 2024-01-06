/*
 * Timer.c
 *
 * Created: 5/12/2023 3:23:29 PM
 *  Author: Mohamed
 */ 

#define F_CPU 1000000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "STD_Macros.h"
#include "Timer0.h"

volatile int counter = 0;
volatile int MaxCount = 0;
volatile int PreloadValue = 0;



void TIMER0_INIT(int DesiredTime)
{	
	sei();						//Global Interrupt EN
	SETBIT(TCCR0,CS00);
	SETBIT(TCCR0,CS01);			//Prescaller = 8
	CLEARBIT(TCCR0,CS02);
	SETBIT(TIMSK,TOIE0);		//Interrupt EN
	
	int cal = 0;
	cal = NUMOVFT0(DesiredTime);
	MaxCount = cal/10;
	PreloadValue = PRELOAD(cal);
}

void TIMER0CTC_INIT(int DesiredTime)
{
	OCR0 = CTCvalue;
	sei();						//Global Interrupt EN
	SETBIT(TCCR0,CS00);
	SETBIT(TCCR0,CS01);			//PS = 8
	CLEARBIT(TCCR0,CS02);
	SETBIT(TIMSK,OCIE0);		//Interrupt EN
	
	SETBIT(TCCR0,WGM01);
	
	int cal = 0;
	cal = NUMOVFT0CTC(DesiredTime);
	MaxCount = cal/10;
	PreloadValue = PRELOADCTC(cal);
}

void TIMER0_FPWM_INIT()
{
	SETBIT(TCCR0,WGM00);
	SETBIT(TCCR0,WGM01);
	SETBIT(TCCR0,COM01);	//non-inverting
	SETBIT(TCCR0,CS00);		//PS 1
	SETBIT(DDRB,3);
}


void TIMER0_PC_PWM_INIT()
{
	SETBIT(TCCR0,WGM00);
	SETBIT(TCCR0,COM01);
	SETBIT(TCCR0,CS00);
	SETBIT(DDRB,3);
}

void Change_DC(float DC)
{
	int temp = (int)(255.0 * DC) ;
	OCR0 = temp;
}


ISR(TIMER0_OVF_vect)
{
	counter++;
}



ISR(TIMER0_COMP_vect)
{
	counter++;
}
