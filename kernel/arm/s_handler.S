@s_handler.S: save current context
@Author: Jiang Xue <jiangx@andrew.cmu.edu>
@Date: 10/25/2013 8:16pm

	.file "s_handler.S"
	.text

	.global	s_handler
s_handler:

    sub sp, sp, #4 @leave space for spsr
    stmfd sp!, {r1, ip}
    mrs	r1, spsr	@get SPSR into registers
	str	r1, [sp, #8]	@store SPSR above registers
	mrs ip, cpsr
	mov r1, #0x00000080 @enable irq
	bic ip, r1
	mov r1, #0x00000040 @disable frq
	orr ip, r1
	msr cpsr, ip
	ldmfd sp!, {r1, ip}
	stmfd sp!, {r0-r12, lr}	@store user's registers
	ldr ip, =global_data
	ldr r8, [ip] @set up r8 correctly
	mov	r1, sp		@pointer to parameters on stack
	ldr	r0, [lr, #-4]	@extract the SWI number
	bic	r0, r0, #0xff000000	@get SWI # by bit-masking
	bl	c_swi_handler	@goto handler
    stmfd sp!, {r1, ip}
    mrs ip, cpsr
    mov r1, #0x80 @disable irq
    orr ip, r1
    msr cpsr, ip
    ldmfd sp!, {r1, ip}
	ldr	r2, [sp, #14*4]	@restore SPSR	
	msr	spsr, r2	@restore SPSR from r2
	add	sp, sp, #4	@start from r1
	ldmfd	sp!,{r1-r12,lr}	@unstack user's registers
	add	sp, sp, #4	@remove space used to store SPSR
	movs	pc, lr		@return from handler

