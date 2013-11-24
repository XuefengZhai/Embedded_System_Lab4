@switchUser: 
@	   switch to user mode and user space
@	   and return to kernel
@Author: Jiang Xue <Jiangx@andrew.cmu.edu>
@Date: 10/25/2013 7:30pm


.file "switchUser.S"
.text
.global switchUser
#include <bits/swi.h>
switchUser:
    @keep irq and fiq disabled.
	@stmfd sp!, {r1, ip}
	@mrs ip, cpsr
	@mov r1, #0x00000080 @disable irq
	@orr ip, r1
	@mov r1, #0x00000040 @disable frq
	@orr ip, r1
	@msr cpsr, ip
	@ldmfd sp!, {r1, ip}
	@stmfd sp!, {r0-r12,lr} @store the registers
	@stmfd sp!, {r0} @store the user_stack pointer to stack
	@mov r0, sp
	@add r0, r0, #4  @the pointer pointing to r0
	@bl saveretAddr 	@save the return address and the svc sp into global vars
	@ldmfd sp!, {r0} @restore the user_stack pointer
	mrs r1, cpsr 	@load cpsr to r1
	mov r2, #0x0000001f
	bic r1, r2 	@clear the last five bits
	mov r2, #0x00000010
	orr r1, r2 	@switch to user mode
	msr cpsr, r1 	@write back to cpsr
	mov sp, r0 	@setup user stack
	mov pc, #0xa0000000 @go to user space

	
