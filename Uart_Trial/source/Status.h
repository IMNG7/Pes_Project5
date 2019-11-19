/*
 * Filename: State_Machine_1.h
 * File Description: The .h file contains the enums for states and events
 * Author: Nitik Satish Gupta and Rakesh Kumar
 */
#ifndef STATUS_H_
#define STATUS_H_
#include <stdint.h>
extern uint8_t Current_event;
extern uint8_t Current_state;
typedef enum state
{
	Temp_Reading,Average_Wait,Temp_Alert,Disconnected,Next_Machine,exit_Machine
}state;
typedef enum event
{
	Complete,Alert,Timeout123,Disconnect,Timeout4,Next
}event;
typedef enum UART_Mode
{
	Polling=1, Interrupt
}UART_Mode;
#endif /* STATUS_H_ */
