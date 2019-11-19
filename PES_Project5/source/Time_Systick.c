/*
 * File Name:Time_Systick.c
 * Description: The Time_systick.c contains the interrupt function and the display function for time elapsed during the execution
 *  Created on: Nov 18, 2019
 *      Author: Nitik gupta and Rakesh Kumar
 */
#include "Time_Systick.h"
#include "Logger.h"
#include "stdlib.h"
#include <stdint.h>
volatile unsigned int t=0,S=0,M=0,H=0,mil=0;
char Hours[10],Mins[10],Seconds[10],Milli[10];
/*	Name: SysTick_Handler()
	Description: The IRQ Handler for the Systic Function
	Inputs: None
	Returns: None
*/
void SysTick_Handler()
{
        t++;
        if(t%160 == 0)	// After every 160 clock, one millisecond will be updated
        {
        	mil++;
        	t=0;
        }
        if(mil==1000)
        {
        	S++;		//After every 1000 milliseconds 1 second will be incremented
        	mil=0;
        }
}
/*	Name: display_time()
	Description: The display function which displays time of the systic
	Inputs: None
	Returns: None
*/
void display_time()
{	M = S/60;
	S = S%60;
	H = M/60;
	M = M%60;
	uitoa(mil, Milli, 10);
	uitoa(S, Seconds, 10);
	uitoa(M, Mins , 10);
	uitoa(H, Hours, 10);
	display_String("\t");
	display_String(Hours);
	display_String(":");
	display_String(Mins);
	display_String(":");
	display_String(Seconds);
	display_String(".");
	display_String(Milli);

}
