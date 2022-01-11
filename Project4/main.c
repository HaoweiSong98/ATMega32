/*
 * project4.c
 *
 * Created: 2021/5/25 2:50:02
 * Author : Yang Lu, Haowei Song
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"

int Mode = 1;
int max_voltage = 0;
int min_voltage = 1023;
int instantaneous_voltage = 0;
long total_voltage = 0;
int times = 0;

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

unsigned int get_sample()
{
	SET_BIT(ADCSRA, ADEN);
	SET_BIT(ADCSRA, ADSC);
	while(GET_BIT(ADCSRA, ADSC));
	return (ADC);
}

void print_result()
{
	char buf1[17] = {0};
	char buf2[17] = {0};
	lcd_clr();
	
	//instantaneous voltage
	int s = get_sample();
	int IV1 = (s/1023.0)*5.0;
	float temp1 = (s/1023.0)*5.0;
	int temp2 = temp1 * 100;
	int IV2 = temp2 % 100;
	
	if (Mode == 1)
	{
		//max voltage
		int Ma1 = (max_voltage/1023.0)*5.0;
		float temp_ma1 = (max_voltage/1023.0)*5.0;
		int temp_ma2 = temp_ma1 * 100;
		int Ma2 = temp_ma2 % 100;
		
		//min voltage
		int Mi1 = (min_voltage/1023.0)*5.0;
		float temp_mi1 = (min_voltage/1023.0)*5.0;
		int temp_mi2 = temp_mi1 * 100;
		int Mi2 = temp_mi2 % 100;
		
		//average voltage
		int ave = total_voltage / times;
		
		int AV1 = (ave/1023.0)*5.0;
		float temp_av1 = (ave/1023.0)*5.0;
		int temp_av2 = temp_av1 * 100;
		int AV2 = temp_av2 % 100;
		
		lcd_pos(0,0);
		sprintf(buf1, "IV:%d.%02d MA:%d.%02d", IV1, IV2, Ma1, Ma2);
		lcd_puts(buf1);
		lcd_pos(1,0);
		sprintf(buf2, "MI:%d.%02d AV:%d.%02d", Mi1, Mi2, AV1, AV2);
		lcd_puts(buf2);
		avr_wait(500);
	}
	else if(Mode == 0)
	{
		lcd_pos(0,0);
		sprintf(buf1, "IV:%d.%02d MA:----", IV1, IV2);
		lcd_puts(buf1);
		lcd_pos(1,0);
		sprintf(buf2, "MI:---- AV:----");
		lcd_puts(buf2);
		avr_wait(500);
	}
	
}

int main(void)
{
	
	CLR_BIT(ADMUX, REFS1);
	SET_BIT(ADMUX, REFS0);
	lcd_init();
	int k;
	while (1)
	{
		int s = get_sample();
		instantaneous_voltage = s;
		k = get_key();
		
		if (k > 0)
		{
			if (k == 1)
			{
				Mode = 1;
			}
			else if (k==2)
			{
				Mode = 0;
			}
		}
		
		if (Mode == 1)
		{
			total_voltage += s;
			times += 1;
			
			if ( s >  max_voltage)
			{
				max_voltage = s;
			}
			else if ( min_voltage > s)
			{
				min_voltage = s;
			}
		}
		
		else if (Mode == 0)
		{
			max_voltage = instantaneous_voltage;
			min_voltage = instantaneous_voltage;
			total_voltage = instantaneous_voltage;
			times = 1;
		}
		
		
		
		
		print_result();
	}

}
