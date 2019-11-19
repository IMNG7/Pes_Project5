/*
 * UARTFunction_polled.c
 *
 *  Created on: Nov 15, 2019
 *      Author: gupta
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include <stdint.h>
#include "Logger.h"
#include "Status.h"
#include "UARTFunction_Interrupt.h"
#include "UARTFunction_polled.h"
#include "led_control.h"
#define BaudRate 115200
#define Sys_Clock CLOCK_GetFreq(kCLOCK_PllFllSelClk)
volatile uint8_t character_recieve,character_transmit;
volatile uint8_t flag=0;
volatile uint8_t txavailable=0;
void delay2()
{
	for(uint16_t j=0;j<1000;j++)
	{
		__asm("NOP");
	}
}
void uart_init_interrupt()
{	led_control(Initialization);
	Log_String("\n\rINSIDE INITIALIZATION OF INTERRUPT");
	uint16_t sbr;
	uint8_t temp;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK/2)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(0) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	// Enable interrupts. Listing 8.11 on p. 234
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);
	// Enable UART receiver and transmitter
		UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

		// Clear the UART RDRF flag
		temp = UART0->D;
		UART0->S1 &= ~UART0_S1_RDRF_MASK;

}
uint8_t transmit_available_interrupt()
{	led_control(Transmit);

	UART0->C2|= UART0_C2_TIE_MASK;

	if(txavailable)
	//(UART0->S1 & UART0_S1_TDRE_MASK)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void transmit_data_interrupt(uint8_t ch)
{	led_control(Transmit);
	if(transmit_available_interrupt())
	{	uint8_t temp=UART0->D;
		UART0->D=ch;
		txavailable=0;
	}
	delay2();
}
uint8_t Receive_available_interrupt()
{	led_control(Recieve);
	if(UART0->S1 && UART0_S1_RDRF_MASK)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
uint8_t Receive_data_interrupt()
{	led_control(Recieve);
	uint8_t temp=UART0->D;
	UART0->C2 |= UART0_S1_RDRF_MASK;
	flag=2;
	PRINTF("\n\r ENTER a Character:");
	while(1)
	{

		if(flag==1)
		{	UART0->S1 &= ~UART0_S1_RDRF_MASK;
			break;

		}
	}
	return character_recieve;
}

void Send_String_interrupt(char * str) {
	// enqueue string
	while (*str != '\0') { // Send characters up to null terminator
		character_transmit=*str++;
		transmit_data_interrupt(character_transmit);
	}
	UART0->C2&=~UART0_C2_TIE_MASK;
}
void UART0_IRQHandler(void) {
	printf("\n\rINSIDE ISR");
	delay2();
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
			// clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			// read the data register to clear RDRF
			//character_recieve = UART0->D;
	}
	if (Receive_available_interrupt())
	{
						// received a character
						character_recieve = UART0->D;
						if(flag==2)
						{
							character_transmit=character_recieve;
							flag=1;
						}
						//PRINTF("%d",character_recieve);
						//delay2();
	}
	if ( UART0->S1 & UART0_S1_TDRE_MASK)
		{ // tx buffer empty
		// can send another character
			txavailable=1;
			UART0->C2&= ~UART0_C2_TIE_MASK;


		}



}
//FILE __stdout;  //Use with printf
//FILE __stdin;		//use with fget/sscanf, or scanf
//

//Retarget the fputc method to use the UART0
//int fputc(int ch, FILE *f){
//	while(!(UART0->S1 & UART_S1_TDRE_MASK) && !(UART0->S1 & UART_S1_TC_MASK));
//	UART0->D = ch;
//	return ch;
//}
//
////Retarget the fgetc method to use the UART0
//int fgetc(FILE *f){
//	while(!(UART0->S1 & UART_S1_RDRF_MASK));
//	return UART0->D;
//}
//
