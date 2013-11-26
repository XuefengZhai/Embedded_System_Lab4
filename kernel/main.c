/*
 * main.c: Kernel main (entry) function
 *
 * Author: Jiang Xue <jiangx@andrew.cmue.du>
 * Date:   11/3/2013 8:13pm
 */

#include <exports.h>
#include <types.h>
#include <arm/psr.h>
#include <arm/reg.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <kernel.h>
#include "kmain.h"
#include <constant.h>
#include <task.h>
#include <sched.h>
#include <assert.h>
#include <device.h>

uint32_t global_data;
volatile unsigned int ptr;
unsigned int SWIorigInstrFirst;
unsigned int SWIorigInstrSecond;
unsigned int SWIHandlerPtr;
unsigned int IRQorigInstrFirst;
unsigned int IRQorigInstrSecond;
unsigned int IRQHandlerPtr;
volatile unsigned long num_of_10ms;

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	/* Add your code here */

	/* Install my swi handler and my irq handler */
	int result = wire_in((unsigned int*)VECTOR_SWI, (unsigned int)&s_handler);
	
	if(result == -1){
		return BAD_CODE;
	}
	
	result = wire_in((unsigned int*)VECTOR_IRQ, (unsigned int)&irq_wrapper);
	
	if(result == -1){
		return BAD_CODE;
	}
	
	/* setup user stack*/
	unsigned int* user_sp = (unsigned int*)USR_STK_BTM;
	
	
	*(user_sp-1) = (unsigned int)0; //null at the bottom of the user stack
	int loopi;
	for(loopi=1;loopi<=argc;loopi++){
	*(user_sp-loopi) = (unsigned int)argv[argc-loopi];
	}
	*(user_sp-argc-1) = (unsigned int)argc;
	
	
	/* setup interrupt controller*/
	uint32_t icmr_content = 0x1;
	reg_write(INT_ICMR_ADDR, icmr_content<<INT_OSTMR_0);
	
	uint32_t iclr_content = 0x0;
	reg_write(INT_ICLR_ADDR, iclr_content);
	
	/*initialize timer*/
	uint32_t oier_content = 0x1;
	reg_write(OSTMR_OIER_ADDR, oier_content);
	
	uint32_t osmr_content = OSTMR_FREQ/FREQ_COEFF;
	reg_write(OSTMR_OSMR_ADDR(0), osmr_content);
	
	uint32_t oscr_content = 0x0;
	reg_write(OSTMR_OSCR_ADDR, oscr_content);
	
	/* go to user space */
	switchUser((unsigned int)(user_sp-argc-1));
	
	
	assert(0);  /*never get here*/
}



