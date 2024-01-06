/*
 * StepperMotor.c
 *
 * Created: 8/23/2023 9:56:56 PM
 *  Author: Mohamed
 */ 
#define F_CPU 1000000ul
#include <avr/io.h>
#include <util/delay.h>
#include "STD_Macros.h"
#include "StepperMotor.h"

// This variable was set to global so after the function ends, it still available
int PerviousAngle = 0;

void StepperMotor_Init()
{
	DDRC = 0x0f;						// This is the PORT for Stepper Motor, it is CONFIG.
	PORTC = 0x09 | (PORTC & 0xf0);		// The first 4 pins of PORTC
}


void StepperMotor_voidDirAngStep(int NewAngle)
{
	char x = 0;			//This is the Variable that will Read the current value of the PORT
	int Angle = (NewAngle - PerviousAngle) / Step;	//Now Angle is the number of times that will iterate in the loop
	int i = 0;
	if (Angle > 0)
	{
		// 1 0 0 1 Initial Position
		// 1 1 0 0 One Step
		// 0 1 1 0 Two Step
		// 0 0 1 1 Three Step
		for (i = 0; i < Angle; i++)
		{
			x = (READBIT(PORTC,3) << 3) + (READBIT(PORTC,2) << 2) + (READBIT(PORTC,1) << 1) + (READBIT(PORTC,0) << 0);
			if (x == 0x09)
			{
				PORTC = 0x0C | (PORTC & 0xf0);
				_delay_ms(100);
			}
			else if (x == 0x0C)
			{
				PORTC = 0x06 | (PORTC & 0xf0);
				_delay_ms(100);
			}
			else if (x == 0x06)
			{
				PORTC = 0x03 | (PORTC & 0xf0);
				_delay_ms(100);
			}
			else if (x == 0x03)
			{
				PORTC = 0x09 | (PORTC & 0xf0);
				_delay_ms(100);
			}
		}
	}
	else if (Angle < 0)
	{
		Angle = -1 * Angle;
		// 0 0 1 1 Initial Position
		// 0 1 1 0 One Step
		// 1 1 0 0 Two Step
		// 1 0 0 1 Initial Position
		for (i = 0; i < Angle; i++)
		{
			x = (READBIT(PORTC,3) << 3) + (READBIT(PORTC,2) << 2) + (READBIT(PORTC,1) << 1) + (READBIT(PORTC,0) << 0);
			if (x == 0x03)
			{
				PORTC = 0x06 | (PORTC & 0xf0);
				_delay_ms(100);
			}
			else if (x == 0x06)
			{
				PORTC = 0x0C | (PORTC & 0xf0);
				_delay_ms(100);
			}
			else if (x == 0x0C)
			{
				PORTC = 0x09 | (PORTC & 0xf0);
				_delay_ms(100);
			}
			else if (x == 0x09)
			{
				PORTC = 0x03 | (PORTC & 0xf0);
				_delay_ms(100);
			}
		}
	}
	else
	{ /* Do Nothing*/ }
	PerviousAngle = NewAngle;
}