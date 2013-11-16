@ getsp.S: Get the sp in supervisor mode
@
@ Author: Xuefeng Zhai <xzhai@andrew.cmu.edu>
@ Date:   Tue, 25 Oct 2012 11:20:33 -0400

	.file	"getsp.S"
	.text

	.global	getsp
getsp:
mov	r0, sp
mov	pc, lr	

