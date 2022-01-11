/*
 * main.c
 *
 * Created: 2021/5/11 1:30:42
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
#define AS1	466
#define B1	494
#define C1	523
#define CS1	554
#define D1	587
#define Ds1	622
#define E1	659
#define F1	699
#define Fs1	740
#define G1	784
#define Gs1	831

#define A2	880
#define AS2	932
#define B2	988
#define C2	1047
#define CS2	1109
#define D2	1175
#define Ds2	1245
#define E2	1319
#define F2	1397
#define Fs2	1480
#define G2	1568
#define Gs2	1661

#define A3	1760
#define AS3	1864
#define B3	1976



int music_number = 0;

struct note{
	int freq;
	int duration;
};


const struct note Happy_Birthday[25] = { {A,H}, {A,H}, {AS,W}, {A,W}, {C,W}, {B,W},
											{A,H}, {A,H}, {AS,W}, {A,W},{D,W}, {C,W},
											{A,H}, {A,H},{G,W}, {E,W},{C,W}, {B,W},{AS,W},
											{F,H}, {F,H}, {E,W}, {C,W},{D,W}, {C,W}
										};
							
const struct note Little_Star[28] = { {C, H}, {C, H}, {G, H}, {G, H}, {A1, H}, {A1, H}, {G, W},
										{F, H}, {F, H}, {E, H}, {E, H}, {D, H}, {D, H}, {C, W},
										{G, H}, {G, H}, {F, H}, {F, H}, {E, H}, {E, H}, {D, W},
										{G, H}, {G, H}, {F, H}, {F, H}, {E, H}, {E, H}, {D, W}};


const struct note Song_of_Joy[29] = {{E,H}, {E,H}, {F,H}, {G,H}, {G,H},
									{F,H}, {E,H} , {D,H}, {C,H}, {C,H}, {D,H},
									{E,H}, {E,W}, {D,Q} ,{D,W},{E,H}, {E,H}, {F,H}, {G,H}, {G,H},
									{F,H}, {E,H} , {D,H}, {C,H}, {C,H}, {D,H},
									{E,H}, {D,W}, {C,Q} ,{C,W}};

					 
				
									
	

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
	lcd_clr();
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
			lcd_pos(1,1);
			sprintf(buf, "Song of Joy");
			lcd_puts(buf);
			int i;
			for (i = 0; i<29; i++)
			{
				k = get_key();
				
				if (k == 4)
				{
					break;
				}
				
				if (k == 2)
				{
					music_number = 1;
					break;
				}
				else if (k == 3)
				{
					music_number = 2;
					break;
				}
				playNote(Song_of_Joy[i].duration, Song_of_Joy[i].freq);
			}
		}
		
		else if ( music_number == 1)
		{
			lcd_clr();
			lcd_pos(0,3);
			sprintf(buf, "Now Play:");
			lcd_puts(buf);
			lcd_pos(1,1);
			sprintf(buf, "Little Star");
			lcd_puts(buf);
			int i;
			for (i = 0; i<28; i++)
			{
				k = get_key();
				
				if (k == 4)
				{
					break;
				}
				
				if (k == 1)
				{
					music_number = 0;
					break;
				}
				else if (k == 3)
				{
					music_number = 2;
					break;
				}

				playNote(Little_Star[i].duration, Little_Star[i].freq);
				
			}
		}
		
		else if ( music_number == 2)
		{
			lcd_clr();
			lcd_pos(0,3);
			sprintf(buf, "Now Play:");
			lcd_puts(buf);
			lcd_pos(1,1);
			sprintf(buf, "Happy Birthday");
			lcd_puts(buf);
			int i;
			for (i = 0; i<25; i++)
			{
				k = get_key();
				
				if (k == 4)
				{
					break;
				}
				
				if (k == 1)
				{
					music_number = 0;
					break;
				}
				else if (k == 2)
				{
					music_number = 1;
					break;
				}

				playNote(Happy_Birthday[i].duration, Happy_Birthday[i].freq);
				
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
		sprintf(buf, "Please select");
		lcd_puts(buf);
		lcd_pos(1,0);
		sprintf(buf, "Song 1 or 2 or 3");
		lcd_puts(buf);
		k = get_key();
		avr_wait(1000);
		
		if (k > 0)
		{
			if (k == 1)
			{
				// First song
				music_number = 0;
				playsong();
			}
			else if (k==2)
			{
				// Second song
				music_number = 1;
				playsong();
			}
			else if (k==3)
			{
				// Third song
				music_number = 2;
				playsong();
			}

		}
		
		
	}
}
