/* File Name: led_control.c
File Description: This file contains implementation for LED initialize and control
Author Name: Nitik Satish Gupta and Rakesh Kumar
*/

#include <Status.h>
#include <stdio.h>
#include <stdint.h>
#include "pin_mux.h"
#include "MKL25Z4.h"
#include <board.h>

/*	Name: delay()
	Description: This function provides a basic delay mechanism.
	Inputs: uint32_t
	Returns: None
*/
void delay()
{			uint16_t nof=10000;
            while(nof!=0) {
              __asm("NOP");
              nof--;
            }
}

/*	Name: Led_Initialize()
	Description: This function when called initializes the LED appropriately.
	Inputs: None
	Returns: None
*/
void Led_Initialize()
{
		gpio_pin_config_t led_pin_config1,led_pin_config2,led_pin_config3;
	    led_pin_config1.pinDirection=kGPIO_DigitalOutput;
	    led_pin_config1.outputLogic= 18u;
	    GPIO_PinInit(GPIOB,18u,&led_pin_config1);
	    led_pin_config2.pinDirection=kGPIO_DigitalOutput;
	    led_pin_config2.outputLogic= 19u;
	    GPIO_PinInit(GPIOB,19u,&led_pin_config2);
	    led_pin_config3.pinDirection=kGPIO_DigitalOutput;
	    led_pin_config3.outputLogic= 1u;
	    GPIO_PinInit(GPIOD,1u,&led_pin_config3);
}

/*	Name: led_control()
	Description: This function is used to control the LED.
	Inputs: mem_status
	Returns: None
*/
void led_control(state a)
{
	if(a==Temp_Reading || a== Average_Wait)
	{
		LED_GREEN_ON();
		LED_RED_OFF();
		LED_BLUE_OFF();
	}
	else if(a==Disconnected || a==exit_Machine)
	{	LED_BLUE_OFF();
		LED_GREEN_OFF();
		LED_RED_ON();
	}
	else if(a==Temp_Alert )
	{	LED_RED_OFF();
		LED_GREEN_OFF();
		LED_BLUE_ON();
		delay(5000000);
	}
}
