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
#define BUFFER_SIZE 5
uint8_t Status=Polling;
uint16_t SIZE = BUFFER_SIZE;
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
    //uart_init_polled();
    //uart_init_interrupt();
    uint8_t data = 0;
    uint8_t choice;
        Send_String_Poll("Hello User,There are two modes you can work in.\n\r");
        Send_String_Poll("1.Polling\n\r");
        Send_String_Poll("2.Interrupt");
        Send_String_Poll("\n\r Please enter the Mode You want to work in:");
        Status=Recieve_polled();
        Transmit_polled(Status);
        Status=Status-48;
        if(Status==1)
        {
        	Send_String_Poll("\n\r Starting with Polling mode.");
        }
        else if(Status==2)
        {
        	Send_String_interrupt("\n\r Starting Interrupt mode.");
        }
        else
        {
        	Send_String_Poll("\n\r Sorry Wrong choice inserted.");
        	Send_String_Poll("\n\r Starting with Polling mode.");
        }
        Send_String_Poll("\n\r=== Circular Buffer Initialization ===\n");
        //printf("\n\rThe size of the initialized structure is -->%d\n\r",cbuf_handle->max);
        //printf("\n\r******\n\rAdding continuous data%d values\n\r", BUFFER_SIZE);
    	//circular_buffer_add(cbuf_handle, 5);

        uint32_t i=0;
    /////For input from UART
        uint8_t char_array[] = {'\0'};
        uint8_t input=0;
        //uint32_t iter = 0;
        do
        {	i++;
        	Send_String_Poll("\n\r Give Values");
        	if(Status==Polling)
        	{
        		char_array[i] = Recieve_polled();
        		Transmit_polled(char_array[i]);
        	}
        	else if(Status==Interrupt)
        	{
        		char_array[i] = Receive_data_interrupt();
        		Transmit_polled(char_array[i]);
        	}
        }while((int)char_array[i]!=13);
    /////For input from UART

    //for(uint32_t i = 0; i < (sizeof(char_array));i++)
    //{
    //	;
    //}
        uint8_t * buffer = malloc(SIZE * sizeof(uint8_t));
        assert(buffer);
        circularbuff_handle_t cbuf_handle = malloc(SIZE * sizeof(circular_buffer_t));

        circular_buffer_init(buffer, SIZE, cbuf_handle);
        cbuf_handle->tail = 0;
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
        	Send_String_Poll("\n\rInside read and remove for loop.");
            //uint8_t data;
        	circular_buffer_remove(cbuf_handle, &data);
        	Send_String_Poll("\n\rThe last data in the buffer that's read and removed is ");
        	Transmit_polled(data);
        }
        Send_String_Poll("\n\rEnter a string to check for character frequency: ");

        //uint8_t rec_char = 0;
        uint8_t char_string[] = {'r','a','k','a','b','b','b','\0'};
        uint32_t a = 0;
    //    while(rec_char != 13)
    //    {
    //    	scanf("%c",&rec_char);
    //    	char_string[a++] = rec_char;
    //    }

    //    printf("\n\rTesting enter value: \n\r");
    //    int val = 0;
    //    scanf("%c",&val);
    //    printf("\n\rThe decimal value for the enter key: %d",val);
    //
    //    printf("\n\rNumber of characters entered: %d", sizeof(char_string));

        int count_chars[sizeof(char_string)] = {'\0'};

        for(a = 0; a < sizeof(char_string); a++)
        {
        	count_chars[char_string[a] - 'a']++;
        }

        printf("\n\rFrequency of %c is %d", char_string[0], count_chars[1]);

        while(1) {
        	//Just an infinite wait;
        }
        return 0 ;
    /* Force the counter to be placed into memory. */
    return 0 ;
}
