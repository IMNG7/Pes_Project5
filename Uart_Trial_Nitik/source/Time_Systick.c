/*
 * File Name:Time_Systick.c
 *
 *  Created on: Nov 18, 2019
 *      Author: Nitik gupta and Rakesh Kumar
 */
#include "Time_Systick.h"
#include "Logger.h"
#include "stdlib.h"
#include <stdint.h>
volatile unsigned int t=0,S=0,M=0,H=0,mil=0;
char Hours[10],Mins[10],Seconds[10],Milli[10];
void SysTick_Handler()
{
        t++;
        //printf("\n\r Inside systick handler%d",time);
        //if(time%160000 == 0)
        if(t%160 == 0)
        {
        	mil++;
        	t=0;
        }
        if(mil==1000)
        {
        	S++;
        	mil=0;
        }
}
void display_time()
{	M = S/60;
	H = M/60;
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
