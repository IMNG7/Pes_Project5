/*
 * File Name: UARTFunction_polled.c
 *	Description: This .c contains all the functions required to make the Program work in interrupt mode.
 *  Created on: Nov 15, 2019
 *  Author: Nitik Gupta and Rakesh Kumar
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "UARTFunction_Interrupt.h"
#include "Logger.h"
#include "Status.h"
#include <stdint.h>
#include "led_control.h"
FILE __stdout;  //Use with printf
FILE __stdin;	//use with fget/sscanf, or scanf
/*	Name: delay1()
	Description: This function provides a basic delay mechanism.
	Inputs: uint32_t
	Returns: None
*/
void delay1()
{
	for(uint16_t j=0;j<1000;j++)
	{
		__asm("NOP");
	}
}
/*	Name: uart_init_polled()
	Description: This function when called initializes the Polling based UART appropriately.
	Inputs: None
	Returns: None
*/
void uart_init_polled()
{	led_control(Initialization);
	Log_String("\n\rINSIDE INITIALIZATION OF POLLED");

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
NVIC_DisableIRQ(UART0_IRQn);
UART0->C2 |= UART_C2_RIE(0)|UART_C2_TIE(0);
	UART0->C2|=UART0_C2_TE_MASK  | UART0_C2_RE_MASK ;
	temp = UART0->D;
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

}
/*	Name: transmit_available()
	Description: Checks whether transmit is available or not
	Inputs: None
	Returns: Integer that show it is available or not
*/
uint8_t transmit_available()
{	led_control(Transmit);
	while(!(UART0->S1 & UART0_S1_TDRE_MASK));
		return 1;

}
/*	Name: transmit_data()
	Description: Transmits data to Uart
	Inputs: Value to be transmitted
	Returns: None
*/
void transmit_data(uint16_t ch)
{	led_control(Transmit);
		UART0->D=ch;
}
/*	Name: transmit_polled()
	Description: Polling based Transmit
	Inputs: Value to be transmitted
	Returns: None
*/
void Transmit_polled(uint16_t ch)
{	led_control(Transmit);
	if(transmit_available())
	{
		transmit_data(ch);
	}
}
/*	Name: Receive_available()
	Description: Checks whether receive is available or not
	Inputs: None
	Returns: Integer that show it is available or not
*/
uint8_t Receive_available()
{	led_control(Recieve);
	while(!(UART0->S1 & UART0_S1_RDRF_MASK));
	return 1;
}
/*	Name: Receive_data()
	Description: receives data from Uart
	Inputs: Value to be receives
	Returns: None
*/
uint8_t Receive_data()
{	led_control(Recieve);
		return UART0->D;
}
/*	Name: transmit_polled()
	Description: Polling based Recieve function
	Inputs: Value to be transmitted
	Returns: None
*/
char Recieve_polled()
{	led_control(Recieve);
	uint8_t temp=UART0->D;
	char ch;
	if(Receive_available())
	{
		ch=UART0->D;
	}
	return ch;
}
//Retarget the fputc method to use the UART0
int fputc(int ch, FILE *f)
{
	while(!(UART0->S1 & UART_S1_TDRE_MASK) && !(UART0->S1 & UART_S1_TC_MASK));
	UART0->D = ch;
	return ch;
}

//Retarget the fgetc method to use the UART0
int fgetc(FILE *f)
{
	while(!(UART0->S1 & UART_S1_RDRF_MASK));
	return UART0->D;
}
/*	Name: Send_String_polled()
	Description: Sends the string value to the polling mode
	Inputs: Value to be sent
	Returns: None
*/
void Send_String_Poll(uint8_t * str) {
	while (*str != '\0') { // Send characters up to null terminator
		Transmit_polled(*str++);
		delay1();
	}
}
