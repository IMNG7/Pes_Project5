/*
 * UARTFunction_Interrupt.h
 *
 *  Created on: Nov 15, 2019
 *      Author: gupta
 */
#include <stdint.h>
#ifndef UARTFUNCTION_INTERRUPT_H_
#define UARTFUNCTION_INTERRUPT_H_

void uart_init_interrupt();
uint8_t transmit_available_interrupt();
void transmit_data_interrupt(uint8_t ch);
uint8_t Receive_available_interrupt();
void Send_String_interrupt(char * str);
uint8_t Receive_data_interrupt();


#endif /* UARTFUNCTION_INTERRUPT_H_ */
