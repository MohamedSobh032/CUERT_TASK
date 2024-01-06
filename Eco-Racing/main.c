/*
 * Eco-Racing.c
 *
 * Created: 8/23/2023 3:48:10 PM
 * Author : Mohamed
 */ 

#define  F_CPU 1000000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "STD_Macros.h"
#include "LCD.h"
#include "UART.h"
#include "Timer0.h"
#include "Timer2.h"
#include "StepperMotor.h"



int main(void)
{
	// Initialize Stepper Motor
	StepperMotor_Init();
	// Initialize LCD
	LCD_INIT();
	// Initialize Timer 2 that controls LCD
	Timer2_INIT(750);			// Due to the delay that is caused by LCD, I've set the timer to interrupt every 750 ms
	// Initialize the PWM to Control Motor Speed
	TIMER0_FPWM_INIT();
	// Initialize UART to communicate with the PC
	UART_INIT(12);
	// Initialize the pins that will control motor directions
	SETBIT(DDRA,0);			// Clockwise Pin
	SETBIT(DDRA,1);			// Anti-Clockwise Pin
		
	//UART VARIABLES
	char x[9];
	char PervX[5];
		
	// Flag that the motor is on
	SETBIT(DDRC,7);
	
	//INITIAL CONDITIONS
	x[0] = '0';
	x[1] = '0';
	x[2] = '0';
	x[3] = 'F';
	x[4] = '0';
	x[5] = '0';
	x[6] = 'R';
	x[7] = 'E';
	PervX[0] = '9';
	PervX[1] = '9';
	PervX[2] = '9';
	PervX[4] = '9';
	PervX[5] = '9';
		
	while (1)
	{	// Read input from the user
		UART_GetString(x);
		// Check Format
		if ( (x[0] >= '0' && x[0] <= '9') && (x[1] >= '0' && x[1] <= '9') && (x[2] >= '0' && x[2] <= '9') && (x[3] == 'F' || x[3] == 'B') && (x[4] >= '0' && x[4] <= '9') && (x[5] >= '0' && x[5] <= '9') && (x[6] == 'R' || x[6] == 'L') && (x[7] == 'E') )
		{
			// LED FLAG
			SETBIT(PORTC,7);
			// Check that the angle and speed did not change, if it is changed, turn it into an integer
			if (x[0] != PervX[0])
			{x[0] = x[0] - '0';}
			if (x[1] != PervX[1])
			{x[1] = x[1] - '0';}
			if (x[2] != PervX[2])
			{x[2] = x[2] - '0';}
			if (x[4] != PervX[3])
			{x[4] = x[4] - '0';}
			if (x[5] != PervX[4])
			{x[5] = x[5] - '0';}
			speed = (100 * x[0]) + (10 * x[1]) + (x[2]);	// multiple the most significant bit by 100, middle significant by 10
			angle = (10 * x[4]) + x[5];				// multiple the most significant bit by 10
			// Check the value of the speed
			if (speed <= 100)
			{
				Change_DC(speed/100);
				// Check the forward or backward bit
				if (x[3] == 'F')
				{
					// Rotate Forward by H-Bridge
					CLEARBIT(PORTA,0);
					SETBIT(PORTA,1);
				}
				else if (x[3] == 'B')
				{
					// Rotate Backward by H-Bridge
					SETBIT(PORTA,0);
					CLEARBIT(PORTA,1);
				}
			}
			
			if (angle <= 45)
			{
				if (x[6] == 'R')
				{StepperMotor_voidDirAngStep(-1*angle);}
				else
				{StepperMotor_voidDirAngStep(angle);}
			}
		}
		else
		{
			// LED FLAG
			CLEARBIT(PORTC,7);
		}
		PervX[0] = x[0];
		PervX[1] = x[1];
		PervX[2] = x[2];
		PervX[3] = x[4];
		PervX[4] = x[5];	
	}
}

