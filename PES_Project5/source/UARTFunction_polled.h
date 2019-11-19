/*
 * File Name: UARTFunction_polled.h
 *	Description: This .h contains all the function definitions required to make the Program work in interrupt mode.
 *  Created on: Nov 15, 2019
 *  Author: Nitik Gupta and Rakesh Kumar
 */
#ifndef UARTFUNCTION_POLLED_H_
#define UARTFUNCTION_POLLED_H_

void uart_init_polled();
uint8_t transmit_available();
void transmit_data(uint16_t ch);
void Transmit_polled(uint16_t ch);
uint8_t Receive_available();
uint8_t Receive_data();
char Recieve_polled();
void Send_String_Poll(uint8_t * str);
#define UART_OVERSAMPLE_RATE 	(16)		// Oversampling rate of the Baud Rate Generator
#define BUS_CLOCK 				(24e6)		// Bus Clock for the UART
#define SYS_CLOCK				(48e6)		// System Clock for the UART
#define baud_rate 115200					// Baud Rate for the UART
#endif /* UARTFUNCTION_POLLED_H_ */
