/* 
 * main.c
 * 
 * Created: 2021/4/14 19:38:01
 * Author: Yang Lu, Haowei Song
 */

#include "avr.h"

int main(void)
{
	// SET_BIT is let something be 1(output)
	// CLR_BIT is let something be 0(input)
	SET_BIT(DDRB, 0); // PB0 is output
	CLR_BIT(DDRB, 1); // PB1 is input
	while(1)
	{
		if((GET_BIT(PINB, PB1)) == 0)  // Push button, if get 00000010 then button is ON
		{
			SET_BIT(PORTB, 0); // LED ON
			avr_wait(500);
			CLR_BIT(PORTB, 0); // LED OFF
			avr_wait(500);
		}
		else
		{
			CLR_BIT(PORTB, 0); // LED OFF
	
		}
	}
	return 0;
}