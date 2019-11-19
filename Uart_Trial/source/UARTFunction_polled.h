/*
 * UARTFunction_polled.h
 *
 *  Created on: Nov 15, 2019
 *      Author: gupta
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
#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 						(24e6)
#define SYS_CLOCK							(48e6)
#define baud_rate 115200
#endif /* UARTFUNCTION_POLLED_H_ */
