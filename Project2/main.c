/*
 * main.c
 *
 * Created: 2021/4/28 7:02:15
 *  Author: Yang Lu, Haowei Song
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"

int change_date = 0;

struct DataT{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};


void initial_time(struct DataT *dt){
	dt->year = 2021;
	dt->month = 4;
	dt->day = 28;
	dt->hour = 23;
	dt->minute = 32;
	dt->second = 3;
}

void advance_dt(struct DataT *dt)
{
	dt->second++;
	if (dt->second == 60)
	{
		dt->second = 0;
		dt->minute++;
		if(dt->minute == 60)
		{
			dt->minute = 0;
			dt->hour++;
			if(dt->hour == 24)
			{
				dt->hour = 0;
				dt->day++;
				if (dt->year % 4 == 0 || dt->year % 400 == 0)   // leap year
				{
					if (dt->month == 2 && dt->day > 29)
					{
						dt->month++;
						dt->day = 1;
					}
					
				}
				else
				{
					if (dt->month == 2 && dt->day > 28 )
					{
						dt->month++;
						dt->day = 1;
					}
					else if(dt->month == 4 || dt->month == 6 || dt->month == 9 || dt->month == 11)
					{
						if(dt->day>30)
						{
							dt->month++;
							dt->day = 1;
						}
					}
					else
					{
						if(dt->day>31)
						{
							dt->month++;
							dt->day = 1;
						}
					}
				}
					if(dt->month > 12)
					{
						dt->year++;
						dt->month = 1;
					}
			}
		}
	}	
}

int is_pressed(int r, int c) {
	DDRC = 0;
	PORTC = 0;			//SET ALL TO N/C
	
	SET_BIT(DDRC, r);	//set row to 0
	CLR_BIT(PORTC, r);
	
	CLR_BIT(DDRC, c+4); // set column to w1
	SET_BIT(PORTC, c+4);
	avr_wait(1);
	if (GET_BIT(PINC, c+4) == 0)
	{
		return 1;
	}

	return 0;
}



int get_key()    
{
	int r, c;
	for (r=0; r<4; r++)
	{
		for (c=0; c<4; c++)
		{
			if (is_pressed(r, c))  // if pressed, we can know the exactly number in key pad
			{
				return r*4+c+1;	// return (1 - 16)
			}
		}
	}
	return 0;
}

void setting_date_mode(struct DataT *dt)
{
	lcd_clr();
	char buf[17] = {0};
	char keypad[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '/', 'D'};
	char output[17] = {0};
	lcd_pos(0,2);
	sprintf(buf, "DATE SETTING");
	lcd_puts(buf);
	int past_year = dt->year;
	int past_month = dt->month;
	int past_day = dt->day;
	int number = 0;
	int k;
	
	while (1)
	{
		k = get_key();
		avr_wait(250);
		if (k >  0 && k != 13)
		{
			output[number] = keypad[k-1];
			number++;
		}
		lcd_pos(1,3);
		sprintf(buf, "%s", output);
		lcd_puts(buf);
		
		if (k == 13)
		{
			if (output[2] == '/' && output[5] == '/')
			{
				int new_year = (output[6] - '0') * 1000 +  (output[7] - '0') * 100 + (output[8] - '0') * 10 + (output[9] - '0');
				int new_day = (output[3] - '0') * 10 +  (output[4] - '0');
				int new_month = (output[0] - '0') * 10 +  (output[1] - '0');
				
				if (new_day < 31 &&  new_month < 12)
				{
					dt->year = new_year;
					dt->month = new_month;
					dt->day = new_day;
					change_date = 1;
				}
			}
			break;
		}
	}
}

void setting_time_mode(struct DataT *dt)
{
	lcd_clr();
	char buf[17] = {0};
	char keypad[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', ':', 'D'};
	char output[17] = {0};
	lcd_pos(0,2);
	sprintf(buf, "TIME SETTING");
	lcd_puts(buf);
	int past_hour = dt->hour;
	int past_minute = dt->minute;
	int past_second = dt->second;
	int number = 0;
	int k;
	
	while (1)
	{
		k = get_key();
		avr_wait(250);
		if (k >  0 && k != 13)
		{
			output[number] = keypad[k-1];
			number++;
		}
		lcd_pos(1,3);
		sprintf(buf, "%s", output);
		lcd_puts(buf);
		
		if (k == 13)
		{
			if (change_date == 0)
			{
				break;
			}
			if (output[2] == ':' && output[5] == ':')
			{
				int new_hour = (output[0] - '0') * 10 +  (output[1] - '0');
				int new_minute = (output[3] - '0') * 10 +  (output[4] - '0');
				int new_second = (output[6] - '0') * 10 +  (output[7] - '0');
				if ( new_hour < 24 &&  new_minute < 60 && new_second < 60 )
				{
					dt->hour = new_hour;
					dt->minute = new_minute;
					dt->second = new_second;
				}
			}
			break;
		}
	}
}

void display_military(const struct DataT *dt)
{
	lcd_clr();
	char buf[17] = {0};
	lcd_pos(0,3);
	sprintf(buf, "%02d/%02d/%04d", dt->month, dt->day, dt->year);
	lcd_puts(buf);
	lcd_pos(1,4);
	sprintf(buf, "%02d:%02d:%02d", dt->hour, dt->minute, dt->second);
	lcd_puts(buf);
}

void display(const struct DataT *dt)
{
	lcd_clr();
	char buf[17] = {0};
	lcd_pos(0,3);
	sprintf(buf, "%02d/%02d/%04d", dt->month, dt->day, dt->year);
	lcd_puts(buf);
	lcd_pos(1,4);
	if (dt->hour > 12)
	{
		sprintf(buf, "%02d:%02d:%02d PM", dt->hour - 12, dt->minute, dt->second);
	}
	else
	{
		sprintf(buf, "%02d:%02d:%02d AM", dt->hour, dt->minute, dt->second);
	}
	lcd_puts(buf);
}

int main(void)
{
	struct DataT dt;
	lcd_init();
	initial_time(&dt);
	int k;
	int military = 1;
	int date_setting = 0;
	while (1)
	{
		k = get_key();
		avr_wait(1000);
		advance_dt(&dt);
		
		if (k > 0)
		{
			if (k == 4)
			{
				date_setting = 1;
			}
			else if (k==12)
			{
				military = 1;
			}
			else if (k==16)
			{
				military = 0;
			}
		}
		
		
		if (military)
		{
			display_military(&dt);
		}
		else
		{
			display(&dt);
		}
		
		if (date_setting)
		{
			setting_date_mode(&dt);
			setting_time_mode(&dt);
			date_setting = 0;
		}
		
	}
	
	
	
	
}

// A: SETTING
// C: military time
// D: AM/PM 
// *: YES
// #: / or :