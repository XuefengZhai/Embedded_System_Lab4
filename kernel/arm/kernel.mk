ARM_OBJS := reg.o psr.o s_handler.o int_asm.o c_irq_handler.o c_swi_handler.o wire_in.o 
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
