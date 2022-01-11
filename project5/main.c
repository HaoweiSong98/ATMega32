/*
 * project5.c
 *
 * Created: 2021/6/6 7:27:59
 * Author : Yang Lu, Haowei Song
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"

#define W 8
#define H 4
#define Q 2
//#define E 8

#define A	220
#define AS	233
#define B	247
#define C	262
#define CS	277
#define D	294
#define Ds	311
#define E	330
#define F	349
#define Fs	370
#define G	392
#define Gs	415

#define A1	440

int music_number = 0;

struct note{
	int freq;
	int duration;
};

					
struct note Song1[29] = {};

struct note Song2[28] = {};

struct note Song3[25] = {};

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

void playNote(int duration, int freq)
{
	float period = 5000.0 / freq;
	int TH = period / 2;
	int TL = period - TH ;
	int k =  duration*1000 / period;

	int i;
	for (i=0; i < k; ++i)
	{
		SET_BIT(PORTA,0);
		avr_wait(TH);
		CLR_BIT(PORTA,0);
		avr_wait(TL);
	}

}

void playsong()
{
	char buf[17] = {0};
	int k;
	while(1)
	{
		if (k == 4)
		{
			break;
		}
		if ( music_number == 0)
		{
			lcd_clr();
			lcd_pos(0,3);
			sprintf(buf, "Now Play:");
			lcd_puts(buf);
			lcd_pos(1,4);
			sprintf(buf, "Song 1");
			lcd_puts(buf);
			int i;
			for (i = 0; i<29; i++)
			{
				k = get_key();
				
				if (k == 4)    // press A to break
				{
					break;
				}
				
				playNote(Song1[i].duration, Song1[i].freq);
			}
		}
		
		else if ( music_number == 1)
		{
			lcd_clr();
			lcd_pos(0,3);
			sprintf(buf, "Now Play:");
			lcd_puts(buf);
			lcd_pos(1,4);
			sprintf(buf, "Song 2");
			lcd_puts(buf);
			int i;
			for (i = 0; i<28; i++)
			{
				k = get_key();
				
				if (k == 4)     // press A to break
				{
					break;
				}
				
				playNote(Song2[i].duration, Song2[i].freq);
				
			}
		}
		
		else if ( music_number == 2)
		{
			lcd_clr();
			lcd_pos(0,3);
			sprintf(buf, "Now Play:");
			lcd_puts(buf);
			lcd_pos(1,4);
			sprintf(buf, "Song 3");
			lcd_puts(buf);
			int i;
			for (i = 0; i<25; i++)
			{
				k = get_key();
				
				if (k == 4)    // press A to break
				{
					break;
				}

				playNote(Song3[i].duration, Song3[i].freq);
				
			}
		}
	}
	
}

void recordsong()
{
	lcd_clr();
	int Note[16] =    { 220, 233, 247,  0,  262,  277, 294,  0,  311,  330, 349,  0,  370,  392,  415,  440  };
	//char Note[16] = { 'A', 'AS', 'B', '/', 'C', 'CS', 'D', '/', 'DS', 'E', 'F', '/', 'FS', 'G', 'GS', 'A1' };
	//                  1    2     3    A    4     5    6    B     7     8     9    C    *     0    #     D
	char buf[17] = {0};
	int output[17] = {0};
	int x;

	if ( music_number == 0)
	{
		lcd_clr();
		lcd_pos(0,3);
		sprintf(buf, "Now Record:");
		lcd_puts(buf);
		lcd_pos(1,4);
		sprintf(buf, "Song 1");
		lcd_puts(buf);
		int i;
		for (i = 0; i<29; i++)
		{
			
			while (1)
			{
				x = get_key();
				avr_wait(500);
				if (x > 0 )
				{
					break;
				} 
			}

			if (x == 4)
			{
				break;
			}

			output[0] = Note[x-1];

			lcd_clr();
			lcd_pos(0,3);
			sprintf(buf, "Now Note:");
			lcd_puts(buf);
			lcd_pos(1,4);
			sprintf(buf, "%d", output[0]);
			lcd_puts(buf);

			
			if ( Note[x-1] != 0)
			{
				Song1[i].duration = Note[x-1];
				Song1[i].freq = H;
			} 
			
			
		}
	}
	
	else if ( music_number == 1)
	{
		lcd_clr();
		lcd_pos(0,3);
		sprintf(buf, "Now Record:");
		lcd_puts(buf);
		lcd_pos(1,4);
		sprintf(buf, "Song 2");
		lcd_puts(buf);
		int i;
		for (i = 0; i<28; i++)
		{
			
			while (1)
			{
				x = get_key();
				avr_wait(500);
				if (x > 0 )
				{
					break;
				} 
			}

			if (x == 4)
			{
				break;
			}

			output[0] = Note[x-1];

			lcd_clr();
			lcd_pos(0,3);
			sprintf(buf, "Now Note:");
			lcd_puts(buf);
			lcd_pos(1,4);
			sprintf(buf, "%d", output[0]);
			lcd_puts(buf);

			
			if ( Note[x-1] != 0)
			{
				Song2[i].duration = Note[x-1];
				Song2[i].freq = H;
			} 
			
			
		}
	}
	
	else if ( music_number == 2)
	{
		lcd_clr();
		lcd_pos(0,3);
		sprintf(buf, "Now Record:");
		lcd_puts(buf);
		lcd_pos(1,4);
		sprintf(buf, "Song 3");
		lcd_puts(buf);
		int i;
		for (i = 0; i<25; i++)
		{
			
			while (1)
			{
				x = get_key();
				avr_wait(500);
				if (x > 0 )
				{
					break;
				} 
			}

			if (x == 4)
			{
				break;
			}

			output[0] = Note[x-1];

			lcd_clr();
			lcd_pos(0,3);
			sprintf(buf, "Now Note:");
			lcd_puts(buf);
			lcd_pos(1,4);
			sprintf(buf, "%d", output[0]);
			lcd_puts(buf);

			
			if ( Note[x-1] != 0)
			{
				Song3[i].duration = Note[x-1];
				Song3[i].freq = H;
			} 
			
			
		}
	}
	
}


int main(void)
{
	lcd_init();
	SET_BIT(DDRA,0);
	int k;
	
	while(1)
	{
		lcd_clr();
		char buf[17] = {0};
		lcd_pos(0,2);
		sprintf(buf, "Play: 1");
		lcd_puts(buf);
		lcd_pos(1,0);
		sprintf(buf, "Record: 2");
		lcd_puts(buf);
		k = get_key();
		avr_wait(1000);
		
		if (k > 0)
		{
			if (k == 1)
			{
				lcd_clr();
				while(1)
				{
					int g;
					// Play song
					lcd_pos(0,2);
					sprintf(buf, "Please play:");
					lcd_puts(buf);
					lcd_pos(1,0);
					sprintf(buf, "Song 1 or 2 or 3");
					lcd_puts(buf);
					g = get_key();
					avr_wait(1000);
					if (g > 0)
					{
						if (g == 1)
						{
							// First song
							music_number = 0;
							playsong();
						}
						else if (g==2)
						{
							// Second song
							music_number = 1;
							playsong();
						}
						else if (g==3)
						{
							// Third song
							music_number = 2;
							playsong();
						}
						break;
					}
				}
			}
			else if (k == 2)
			{
				lcd_clr();
				while(1)
				{
					// record song
					int g;
					lcd_pos(0,2);
					sprintf(buf, "Please record:");
					lcd_puts(buf);
					lcd_pos(1,0);
					sprintf(buf, "Song 1 or 2 or 3");
					lcd_puts(buf);
					g = get_key();
					avr_wait(1000);
					if (g > 0)
					{
						if (g == 1)
						{
							// First song
							music_number = 0;
							recordsong();
						}
						else if (g == 2)
						{
							// Second song
							music_number = 1;
							recordsong();
						}
						else if (g == 3)
						{
							// Third song
							music_number = 2;
							recordsong();
						}
						break;
					}

				}
				
			}
				
		}
		
		
	}
}


