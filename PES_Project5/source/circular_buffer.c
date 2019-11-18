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
#define BUFFER_SIZE 5
extern uint16_t SIZE;

bool status_flag;

//Function to carry out the initialization of the buffer structure
void circular_buffer_init(uint8_t * buffer, size_t size, circularbuff_handle_t cbuf_struct_handle)
{
	assert(cbuf_struct_handle);
	status_flag = circular_buffer_reset(cbuf_struct_handle);		//could add test cases
	cbuf_struct_handle->buffer = buffer;
	cbuf_struct_handle->max = size;
	cbuf_struct_handle->full = 0;
	cbuf_struct_handle->tail = 0;
	cbuf_struct_handle->head = 0;
}

static void advance_pointer(circularbuff_handle_t cbuf_struct_handle)
{
	assert(cbuf_struct_handle);

	if(1 == cbuf_struct_handle->full)
    {
		//cbuf_struct_handle->tail = (cbuf_struct_handle->tail + 1) % cbuf_struct_handle->max;
		printf("\n\rin advance_pointer full--> %d",cbuf_struct_handle->full);
		cbuf_struct_handle->tail = (cbuf_struct_handle->tail + 1) % SIZE;
		printf("\n\rWould realloc and Increase capacity by 20, in future");
		//realloc(cbuf_struct_handle,10*sizeof(circular_buffer_t));
		//malloc()
    }

	//cbuf_struct_handle->head = (cbuf_struct_handle->head + 1) % cbuf_struct_handle->max;
	cbuf_struct_handle->head = (cbuf_struct_handle->head + 1) % SIZE;
	printf("\n\rcbuf_struct_handle->head = %d",cbuf_struct_handle->head);

//	if(cbuf_struct_handle->head == cbuf_struct_handle->tail)
//	{
//		printf("\n\rhead = tail");
//	}
	// We mark full because we will advance tail on the next time around
	cbuf_struct_handle->full = (cbuf_struct_handle->head == cbuf_struct_handle->tail);
	printf("\n\rout advance_pointer full--> %d",cbuf_struct_handle->full);

}

Error circular_buffer_add(circularbuff_handle_t cbuf_struct_handle, uint8_t data)
{
	cbuf_struct_handle->buffer[cbuf_struct_handle->head] = data;
	printf("\n\rChecking data inside the function circular_buffer_add-->%c",cbuf_struct_handle->buffer[cbuf_struct_handle->head]);
	advance_pointer(cbuf_struct_handle);
	if(cbuf_struct_handle->buffer[cbuf_struct_handle->head])
	{
		return SUCCESS;
	}
	{
		return FAIL;
	}
}

static void retreat_pointer(circularbuff_handle_t cbuf_struct_handle)
{
	assert(cbuf_struct_handle);

	cbuf_struct_handle->full = 0;
	//cbuf_struct_handle->tail = (cbuf_struct_handle->tail + 1) % cbuf_struct_handle->max;
	cbuf_struct_handle->tail = (cbuf_struct_handle->tail + 1) % SIZE;
}

uint8_t circular_buffer_remove(circularbuff_handle_t cbuf_struct_handle, uint8_t * data)
{
	assert(cbuf_struct_handle && data && cbuf_struct_handle->buffer);
	int r = -1;
	if(!circular_buf_empty(cbuf_struct_handle))
	{
	    *data = cbuf_struct_handle->buffer[cbuf_struct_handle->tail];
	    retreat_pointer(cbuf_struct_handle);
	    r = 0;
	}
	printf("\n\rIn remove function: cbuf_struct_handle->tail = %d",cbuf_struct_handle->tail);
	return r;
}


Error circular_buffer_reset(circularbuff_handle_t cbuf_struct_handle)
{
    assert(cbuf_struct_handle);
    cbuf_struct_handle->head = 0;
    cbuf_struct_handle->tail = 0;
    cbuf_struct_handle->full = 0;
    cbuf_struct_handle->count = 0;
    printf("The buffer has been reseted!!");
    return SUCCESS;
}

Error circular_buffer_full(circularbuff_handle_t cbuf_struct_handle)
{
	assert(cbuf_struct_handle);
	if(1 == (cbuf_struct_handle->full))
	{
		printf("\n\rThe buffer is full, will loop-back and overwrite henceforth!!");
		return SUCCESS;
	}
	else
	{
		printf("\n\rThe buffer still has some space!!");
	}
	return SUCCESS;
}

size_t circular_buffer_capacity(circularbuff_handle_t cbuf_struct_handle)
{
	assert(cbuf_struct_handle);
	return cbuf_struct_handle->max;
}

bool circular_buf_empty(circularbuff_handle_t cbuf_struct_handle)
{
	assert(cbuf_struct_handle);
	printf("\n\rInside buffer empty function");
//	printf("\n\rcbuf_struct_handle->full  ---- %d",cbuf_struct_handle->full);
//	printf("\n\rcbuf_struct_handle->head  ---- %d",cbuf_struct_handle->head);
//	printf("\n\rcbuf_struct_handle->tail  ---- %d",cbuf_struct_handle->tail);
    return (!(cbuf_struct_handle->full) && (cbuf_struct_handle->head == cbuf_struct_handle->tail));
}
