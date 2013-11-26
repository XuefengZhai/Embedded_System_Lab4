/* c_irq_handler.c: update the time and restart the timer
 * Author: Jiang Xue <jiangx@andrew.cmu.edu>
 * Date: 2013/11/6 1:56PM
 */

#include <exports.h>
#include <types.h>
#include <arm/psr.h>
#include <arm/reg.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <constant.h>
#include <c_irq_handler.h>
#include <device.h>
#include <sched.h>
void c_irq_handler(){
	

	/* check the source of the interrupt*/
	uint32_t icpr_content = reg_read(INT_ICPR_ADDR);
	if((icpr_content & (0x1<<INT_OSTMR_0)) == 0){
		return;
	}
	
	/* acknowledge the interrupt*/
	reg_write(OSTMR_OSSR_ADDR, 0x1);
	
	/* update the time and restart the timer*/
	num_of_10ms++;
	
	uint32_t osmr_content = OSTMR_FREQ/FREQ_COEFF;
	reg_write(OSTMR_OSMR_ADDR(0), osmr_content);
	
	uint32_t oscr_content = 0x0;
	reg_write(OSTMR_OSCR_ADDR, oscr_content);
	
    /* update the run queue if there is a match for a device */
    dev_update(num_of_10ms * 10);
    
    /* switch to a higher priority task if possible */
    dispatch_save();
    
	return;
}

