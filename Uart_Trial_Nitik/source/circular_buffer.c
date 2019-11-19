/*
 * circular_buffer.c
 *
 *  Created on: 12-Nov-2019
 *      Author: rakma
 */

#include "circular_buffer.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "uCUnit-v1.0.h"
#include "Logger.h"
#include "Status.h"
#include "led_control.h"
#define BUFFER_SIZE 5
extern uint16_t SIZE;

bool status_flag;

//Function to carry out the initialization of the buffer structure
void circular_buffer_init(uint8_t * buffer, size_t size, circularbuff_handle_t cbuf_struct_handle)
{	Log_String("\n\rINSIDE CIRCULAR BUFFER INITIALIZATION");
	UCUNIT_CheckIsNotNull(cbuf_struct_handle);
	assert(cbuf_struct_handle);
	status_flag = circular_buffer_reset(cbuf_struct_handle);		//could add test cases
	if(status_flag==FAIL)
	{
		led_control(Fail_case);
	}
	UCUNIT_CheckIsEqual(SUCCESS,status_flag);
	cbuf_struct_handle->buffer = buffer;
	cbuf_struct_handle->max = size;
	cbuf_struct_handle->full = 0;
	cbuf_struct_handle->tail = 0;
	cbuf_struct_handle->head = 0;
}

static void advance_pointer(circularbuff_handle_t cbuf_struct_handle)
{	Log_String("\n\rINSIDE ADVANCE POINTER");
	UCUNIT_CheckIsNotNull(cbuf_struct_handle);
	assert(cbuf_struct_handle);
	UCUNIT_CheckIsEqual(1,cbuf_struct_handle->full);
	if(1 == cbuf_struct_handle->full)
    {
		//cbuf_struct_handle->tail = (cbuf_struct_handle->tail + 1) % cbuf_struct_handle->max;
		display_String("\n\rin advance_pointer full--> ");
		Print_Data(cbuf_struct_handle->full);
		cbuf_struct_handle->tail = (cbuf_struct_handle->tail + 1) % SIZE;
		display_String("\n\rWould realloc and Increase capacity by 20, in future");
		//realloc(cbuf_struct_handle,10*sizeof(circular_buffer_t));
		//malloc()
		////////////test case to check for buffer full flag
    }

	//cbuf_struct_handle->head = (cbuf_struct_handle->head + 1) % cbuf_struct_handle->max;
	cbuf_struct_handle->head = (cbuf_struct_handle->head + 1) % SIZE;
	display_String("\n\rcbuf_struct_handle->head = ");
	Print_Data(cbuf_struct_handle->head);

//	if(cbuf_struct_handle->head == cbuf_struct_handle->tail)
//	{
//		printf("\n\rhead = tail");
//	}
	// We mark full because we will advance tail on the next time around
	cbuf_struct_handle->full = (cbuf_struct_handle->head == cbuf_struct_handle->tail);
	display_String("\n\rout advance_pointer full--> ");
	Print_Data(cbuf_struct_handle->full);

}

Error circular_buffer_add(circularbuff_handle_t cbuf_struct_handle, uint8_t data)
{	Log_String("\n\rINSIDE CIRCULAR BUFFER ADD");
	UCUNIT_CheckIsNotNull(cbuf_struct_handle);
	cbuf_struct_handle->buffer[cbuf_struct_handle->head] = data;
	display_String("\n\rChecking data inside the function circular_buffer_add--> ");
	Print_Data(cbuf_struct_handle->buffer[cbuf_struct_handle->head]);
	advance_pointer(cbuf_struct_handle);
	UCUNIT_CheckIsEqual(1,cbuf_struct_handle->buffer[cbuf_struct_handle->head]);
	if(cbuf_struct_handle->buffer[cbuf_struct_handle->head])
	{
		return SUCCESS;
	}
	{
		led_control(Fail_case);
		return FAIL;
	}
}

static void retreat_pointer(circularbuff_handle_t cbuf_struct_handle)
{	Log_String("\n\rINSIDE RETREAT POINTER");
	UCUNIT_CheckIsNotNull(cbuf_struct_handle);
	assert(cbuf_struct_handle);
	cbuf_struct_handle->full = 0;
	//cbuf_struct_handle->tail = (cbuf_struct_handle->tail + 1) % cbuf_struct_handle->max;
	cbuf_struct_handle->tail = (cbuf_struct_handle->tail + 1) % SIZE;
}

uint8_t circular_buffer_remove(circularbuff_handle_t cbuf_struct_handle, uint8_t * data)
{	Log_String("\n\rINSIDE CIRCULAR BUFFER REMOVE");
	UCUNIT_CheckIsNotNull(cbuf_struct_handle);
	assert(cbuf_struct_handle && data && cbuf_struct_handle->buffer);
	int r = -1;
	if(!circular_buf_empty(cbuf_struct_handle))
	{
	    *data = cbuf_struct_handle->buffer[cbuf_struct_handle->tail];
	    retreat_pointer(cbuf_struct_handle);
	    r = 0;
	}
	UCUNIT_CheckIsEqual(0,r);
	display_String("\n\rIn remove function: cbuf_struct_handle->tail = ");
	Print_Data(cbuf_struct_handle->tail);
	return r;
}

Error circular_buffer_reset(circularbuff_handle_t cbuf_struct_handle)
{	Log_String("\n\rINSIDE CIRCULAR BUFFER RESET");
	UCUNIT_CheckIsNotNull(cbuf_struct_handle);
    assert(cbuf_struct_handle);
    cbuf_struct_handle->head = 0;
    cbuf_struct_handle->tail = 0;
    cbuf_struct_handle->full = 0;
    cbuf_struct_handle->count = 0;
    display_String("The buffer has been reseted!!");
    return SUCCESS;
}

Error circular_buffer_full(circularbuff_handle_t cbuf_struct_handle)
{	Log_String("\n\rINSIDE CIRCULAR BUFFER FULL");
	UCUNIT_CheckIsNotNull(cbuf_struct_handle);
	assert(cbuf_struct_handle);
	UCUNIT_CheckIsEqual(1,cbuf_struct_handle->full);
	if(1 == (cbuf_struct_handle->full))
	{
		display_String("\n\rThe buffer is full, will loop-back and overwrite henceforth!!");
		////////////check for buffer full condition
		return SUCCESS;
	}
	else
	{
		led_control(Fail_case);
		display_String("\n\rThe buffer still has some space!!");
	}
	return SUCCESS;
}

size_t circular_buffer_capacity(circularbuff_handle_t cbuf_struct_handle)
{	Log_String("\n\rINSIDE CIRCULAR BUFFER CAPACITY");
	UCUNIT_CheckIsNotNull(cbuf_struct_handle);
	assert(cbuf_struct_handle);
	return cbuf_struct_handle->max;
}

bool circular_buf_empty(circularbuff_handle_t cbuf_struct_handle)
{	Log_String("\n\rINSIDE CIRCULAR BUFFER EMPTY");
	UCUNIT_CheckIsNotNull(cbuf_struct_handle);
	assert(cbuf_struct_handle);
//	printf("\n\rcbuf_struct_handle->full  ---- %d",cbuf_struct_handle->full);
//	printf("\n\rcbuf_struct_handle->head  ---- %d",cbuf_struct_handle->head);
//	printf("\n\rcbuf_struct_handle->tail  ---- %d",cbuf_struct_handle->tail);
    return (!(cbuf_struct_handle->full) && (cbuf_struct_handle->head == cbuf_struct_handle->tail));
}
bool circular_buffer_valid(circularbuff_handle_t cbuf_struct_handle)
{
	if (NULL == cbuf_struct_handle)
	{
		led_control(Fail_case);
		return 0;
	}
	else
	{
		return 1;
	}
}

bool circular_buffer_init_check(circularbuff_handle_t cbuf_struct_handle)
{
	if(!status_flag)
	{
		led_control(Fail_case);
	}
	return status_flag;
}

void circular_buffer_destroy(circularbuff_handle_t cbuf_struct_handle)
{
	free(cbuf_struct_handle->buffer);
	free(cbuf_struct_handle);
	display_String("\n\rThe circular buffer has been successfully destroyed!!");
}
