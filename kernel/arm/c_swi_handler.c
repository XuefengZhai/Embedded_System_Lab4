/*
 *c_swi_handler.c
 *Author: Long Tu <longt@andrew.cmu.edu>
 *Date: 10/25/2013 7:26pm
 */

#include <types.h>
#include <bits/swi.h>
#include <kernel.h>
#include <swi_handler.h>
#include <constant.h>


/*
 * c_swi_handler translate different swi_num to do different sys calls
 */
void c_swi_handler(unsigned swi_num, unsigned *regs){
	switch(swi_num){
		case READ_SWI:
			//read
			read((int)regs[0], (void *)regs[1], (size_t)regs[2]);
			break;
		case WRITE_SWI:
			//write
			write((int)regs[0],(const void *)regs[1], (size_t)regs[2]);
			break;
		case TIME_SWI:
			//time
			time();
			break;
		case SLEEP_SWI:
			//sleep
			sleep((unsigned long)regs[0]);
			break;
        case CREATE_SWI:
            //create taks
            task_create((tast_t*)regs[0], (size_t)regs[1]);
            break;
        case MUTEX_CREATE:
            //create a mutex
            mutex_create();
            break;
        case MUTEX_LOCK:
            //lock the mutex
            mutex_lock((int)regs[0]);
            break;
        case MUTEX_UNLOCK:
            //unlock the mutex
            mutex_unlock((int)regs[0]);
            break;
        case EVENT_WAIT:
            //put the caller into a sleep queue
            event_wait((unsigned int)regs[0]);
            break;
		default:
			//invalid sys calls
			invalid_syscall((unsigned int)swi_num);
			break;
	}
}


