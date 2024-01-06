
#define  F_CPU 1000000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include "STD_Macros.h"
#include "Timer2.h"
#include "LCD.h"
#include <util/delay.h>
volatile int count_t2=0;
volatile int MaxCount_t2=0;
volatile int Preload_t2=0;

volatile int angle = 0;
volatile float speed = 0;

int seconds = 0;
int minutes = 0;
int hours = 0;
char days = 'A';
void Timer2_INIT(int DesiredT)
{
	SETBIT(TCCR2,CS01);
	SETBIT(TIMSK,TOIE2);
	sei(); //global interrupt enable
	int cal=NOVF(DesiredT);
	Preload_t2=PRELOAD_t2(cal);
	MaxCount_t2=cal/10;
}

void TIMER2_FPWM_INIT()
{
	SETBIT(TCCR2,WGM00);
	SETBIT(TCCR2,WGM01);
	SETBIT(TCCR2,COM01);	//nin-inverting
	SETBIT(TCCR2,CS20);		//PS 1
	SETBIT(DDRD,7);
}

void Change_DC2(float DC)
{
	int temp = (int)(255.0 * DC) ;
	OCR2 = temp;
}


ISR(TIMER2_OVF_vect)
{
	
	count_t2++;
	// when we count the desired time
	if (count_t2 > MaxCount_t2)
	{
		// clear the counter
		count_t2 = 0;
		// increase the seconds by 1
		seconds++;
		// if the seconds is equal to 60, set it to 0 and increase the minutes
		if (seconds == 60)
		{
			seconds = 0;
			minutes++;
			// if the minutes is equal to 60, set it to 0 and increase the hours
			if (minutes == 60)
			{
				minutes = 0;
				hours++;
				// if the hours is equal to 13, set it to 0 and change from the A to P or vice versa
				if (hours == 13)
				{
					hours = 0;
					if (days == 'A')
					{ days = 'P'; }
					else if (days == 'P')
					{ days = 'A'; }
				}
			}
		}
		// Clear the screen before typing again
		LCD_ClearScreen();
		LCD_String("Speed:");
		LCD_Integer((int)speed);
		LCD_String("%Dir:");
		LCD_Integer(angle);
		LCD_send_command(0xc0);
		if (hours == 0)
		{
			LCD_send_data('0');
			LCD_send_data('0');
		}
		else if (hours < 10)
		{
			LCD_send_data('0');
			LCD_Integer(hours);
		}
		else if (hours >= 10)
		{ LCD_Integer(hours); }
		LCD_send_data(':');
		
		if (minutes == 0)
		{
			LCD_send_data('0');
			LCD_send_data('0');
		}
		else if (minutes < 10)
		{
			LCD_send_data('0');
			LCD_Integer(minutes);
		}
		else  if (minutes >= 10)
		{ LCD_Integer(minutes); }
		LCD_send_data(':');
		
		if (seconds == 0)
		{
			LCD_send_data('0');
			LCD_send_data('0');
		}
		else if (seconds < 10)
		{
			LCD_send_data('0');
			LCD_Integer(seconds);
		}
		else if (seconds >= 10)
		{ LCD_Integer(seconds); }
		LCD_send_data(days);
		LCD_send_data('M');
	}
}
