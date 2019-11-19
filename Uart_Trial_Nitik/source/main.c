/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Uart_Trial.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "UARTFunction_polled.h"
#include "UARTFunction_Interrupt.h"
#include "led_control.h"
#include "Logger.h"
#include "Status.h"
#include "Testsuite.h"
#include "System.h"
#include "circular_buffer.h"
#include "uCUnit-v1.0.h"
#include <stdlib.h>
#include <stdbool.h>
#include "Time_Systick.h"
#include "stdlib.h"
#define BUFFER_SIZE 5
uint16_t SIZE = BUFFER_SIZE;
uint8_t Status=Polling;
uint16_t count_chars[26] = {0};
uint16_t count_CHARS[26] = {0};
uint16_t count_num[10] = {0};
void display_report();
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

int main(void) {

  	/* Init board hardware. */

	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();
    Led_Initialize();
    uart_init_polled();
    Log_Enable();
    SysTick_Config(48000000/160000);
    //uart_init_interrupt();
    uint8_t data = 0;
    memset(count_CHARS,0,sizeof(count_CHARS));
    memset(count_chars,0,sizeof(count_chars));
    memset(count_num,0,sizeof(count_num));
    uint8_t choice;
    display_String("Hello User,There are two modes you can work in.\n\r");
    display_String("1.Polling\n\r");
    display_String("2.Interrupt");
    display_String("\n\r Please enter the Mode You want to work in:");
        choice=Recieve_polled();
        Transmit_polled(Status);
        choice=choice-48;
        if(choice==1)
        {
        	display_String("\n\r Starting with Polling mode.");
        	Status=Polling;
        }
        else if(choice==2)
        {
        	display_String("\n\r Starting Interrupt mode.");
        	uart_init_interrupt();
        	Status=Interrupt;
        }
        else
        {
        	display_String("\n\r Sorry Wrong choice inserted.");
        	display_String("\n\r Starting with Polling mode.");
        }
        display_String("\n\r=== Circular Buffer Initialization ===\n");
        //printf("\n\rThe size of the initialized structure is -->%d\n\r",cbuf_handle->max);
        //printf("\n\r******\n\rAdding continuous data%d values\n\r", BUFFER_SIZE);
    	//circular_buffer_add(cbuf_handle, 5);

        int32_t i=-1;
    /////For input from UART
        uint8_t char_array[BUFFER_SIZE] = {'\0'};
        //uint32_t iter = 0;
        do
        {	i++;
        display_String("\n\r Give Values");
        	if(Status==Polling)
        	{
        		char_array[i] = Recieve_polled();
        		Print_Data(char_array[i]);
        	}
        	else if(Status==Interrupt)
        	{
        		char_array[i] = Receive_data_interrupt();
        		Print_Data(char_array[i]);
        	}
        }while((int)char_array[i]!=13);
        /////For input from UART
    //        for(i=0;i<5;i++)
    //        {
    //        	printf("\n\r%c",char_array[i]);
    //        }
            uint8_t * buffer = malloc(SIZE * sizeof(uint8_t));
            ////////////test case for check for input buffer to circular buffer init
            assert(buffer);
            circularbuff_handle_t cbuf_handle = malloc(SIZE * sizeof(circular_buffer_t));
            ////////////test case for check successfull structure malloc for the circular buffer

            bool res = circular_buffer_valid(cbuf_handle);
            if(res)
            {
            	display_String("\n\r The buffer pointer is valid");
            }
            else
            {
            	display_String("\n\r Invalid Buffer pointer");
            }

            circular_buffer_init(buffer, SIZE, cbuf_handle);

            res = circular_buffer_init_check(cbuf_handle);
            if(res)
            {
            	display_String("\n\r The buffer pointer is initialized");
            }
            else
            {
            	display_String("\n\r Couldn't initialize buffer");
            }

            cbuf_handle->tail = 0;

            //Init_SysTick();
            //printf("\n\rSizeof char_array: %d",sizeof(char_array));
            for(uint32_t i = 0; i < 7; i++)
            {
                circular_buffer_add(cbuf_handle, char_array[i]);
                display_String("\n\rData being added char_array[i]: ");
                Print_Data(char_array[i]);
                circular_buffer_full(cbuf_handle);
            }
    //        for(uint8_t i = 0; i < (SIZE+2); i++)
    //        {
    //        	PRINTF("\n\rEnter data to be entered into the circular buffer\n\r");
    //
    //        	scanf("%c",&data);
    //        	circular_buffer_add(cbuf_handle, data);
    //        	circular_buffer_full(cbuf_handle);
    //        }

           // uint8_t data;
            //circular_buffer_remove(cbuf_handle, &data);

            for(uint8_t k = 0; k < (SIZE); k++)
            {
            	circular_buffer_remove(cbuf_handle, &data);
            	//Send_String_Poll("\n\rThe last data in the buffer that's read and removed is ");
                display_String("\n\rdata-> ");
                Print_Data(data);
            	//Transmit_polled(data);
            }
        	circular_buffer_remove(cbuf_handle, &data);

            display_String("\n\rEnter a string to check for character frequency: ");

            uint32_t a = 0;

            display_String("\n\rSizeof char_array: ");
            printf( sizeof(char_array));
            for(a = 0; a < sizeof(char_array); a++)
            {
            	if((char_array[a] >= 'a') && (char_array[a] <= 'z'))
            	{
            		count_chars[char_array[a] - 'a']++;
            	}
            	if((char_array[a] >= 'A') && (char_array[a] <= 'Z'))
            	{
            		count_CHARS[char_array[a] - 'A']++;
            	}
            	if((char_array[a] >= '0') && (char_array[a] <= '9'))
            	{
            		count_num[char_array[a] - '0']++;
            	}
            }
            //display_time();
            display_report();
            //}

            circular_buffer_destroy(cbuf_handle);
        while(1) {
        	//Just an infinite wait;
        }
        return 0 ;
    /* Force the counter to be placed into memory. */
    return 0 ;
}
void display_report()
{	char cha[10];
    uint32_t a = 0;
	for(a='a';a<='z';a++)
	{
	   	if(count_chars[a-'a']!=0)
	   	{
	  		display_String("\n\rFrequency of ");
	  		Print_Data(a);
	  		display_String("is");
	  		itoa(count_chars[a-'a'],cha,10);
	  		display_String(cha);
	   	}
	}

	for(a='A';a<='Z';a++)
	{
	   	if(count_CHARS[a-'A']!=0)
	   	{
	   		display_String("\n\rFrequency of ");
	   		Print_Data(a);
	   		display_String("is");
	   		itoa(count_CHARS[a-'A'],cha,10);
	   		display_String(cha);
	   	}
	}

	for(a='0';a<='9';a++)
	{
	   	if(count_num[a-'0']!=0)
	   	{

	   		display_String("\n\rFrequency of ");
	   		Print_Data(a);
	   		display_String(" is ");
	   		itoa(count_num[a-'0'],cha,10);
	   		display_String(cha);
	   	}
	}
}

