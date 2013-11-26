/* time.c: realize the function of getting current time
 * 	   since the kernel booted up
 * return: time passed since the kernel booted up in milliseconds
 * Author: Jiang Xue <jiangx@andrew.cmu.edu>
 * Time: 11/6/2013 2:24PM
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>
#include <arm/exception.h>
extern volatile unsigned long num_of_10ms;

unsigned long time(){

	enable_interrupts();
	
	unsigned long ret = num_of_10ms * 10;
	
	return ret;

}



/** @brief Waits in a tight loop for atleast the given number of milliseconds.
 *
 * @param millis  The number of milliseconds to sleep.
 *
 * 
 */
void sleep(unsigned long millis){

	enable_interrupts();
		
	/* calculate the expected count of 10ms */
	unsigned long expected_num_of_10ms = num_of_10ms + millis/10;
	
	/* loop until the current count of 10ms 
	 * gets to the expected one 
	 */
	while(num_of_10ms < expected_num_of_10ms){}
	
	return;
	

}
