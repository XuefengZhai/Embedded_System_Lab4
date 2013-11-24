/*
 * hijack the orginal swi_handler and irq_handler
 * author: Johnny Xue <jiangx@andrew.cmu.edu>
 * time: 3:31PM 11/21/2013
 */
#include <constant.h>

int wire_in (unsigned int* VectorTable, unsigned int MyHandler){
	
    
	unsigned int immOffset = 0;
	unsigned int ldrInstr = 0;
	unsigned int ldrMask = 0x0f1ff000;
	unsigned int* JmpTablePtr;
	
    
	
	ldrInstr = *VectorTable;
	
	/*
	 * Check if the instruction is a ldr pc, [pc, #imm12]
	 */
	if((ldrInstr & ldrMask)!=LDR_OPCODE){
        
		return -1;}
    
	ldrMask = 0x00000fff;
	immOffset = ldrMask & ldrInstr;//Get the last 12 bit which is the offset
	
	ldrMask = 0x00800000;
	if((ldrInstr & ldrMask)==0x00000000){//offset is added or substracted
		JmpTablePtr = VectorTable+2-(immOffset>>2);}
	else{
		JmpTablePtr = VectorTable+2+(immOffset>>2);}
    
	/*
	 * save the original instructions
	 * hijack the instructions
	 */
    
	if(VectorTable == (unsigned int*)VECTOR_SWI){
		
		SWIHandlerPtr =  *JmpTablePtr;
		SWIorigInstrFirst = *((unsigned int*)SWIHandlerPtr);
		SWIorigInstrSecond = *(((unsigned int*)SWIHandlerPtr)+1);
        
        /*
         * modify the ldr instruction to ldr pc, [pc, #-4]
         * modify the second instruction to the address of my SWI handler
         */
        
		unsigned int newInstrFirst = (ldrInstr & 0xff7ff000) + (unsigned int)0x4;
		*((unsigned int*)SWIHandlerPtr) = newInstrFirst;
		*(((unsigned int*)SWIHandlerPtr)+1) = MyHandler;
	}
	else if(VectorTable == (unsigned int*)VECTOR_IRQ){
        
		IRQHandlerPtr =  *JmpTablePtr;
		IRQorigInstrFirst = *((unsigned int*)IRQHandlerPtr);
		IRQorigInstrSecond = *(((unsigned int*)IRQHandlerPtr)+1);
        
        /*
         * modify the ldr instruction to ldr pc, [pc, #-4]
         * modify the second instruction to the address of my IRQ handler
         */
        
		unsigned int newInstrFirst = (ldrInstr & 0xff7ff000) + (unsigned int)0x4;
		*((unsigned int*)IRQHandlerPtr) = newInstrFirst;
		*(((unsigned int*)IRQHandlerPtr)+1) = MyHandler;
        
    }
	else{
		return -1;
	}
	
    
	
	
	return 1;
}