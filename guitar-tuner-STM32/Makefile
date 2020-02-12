TEMPLATEROOT = ..

# compilation flags for gdb

CFLAGS  = -O1 -g
ASFLAGS = -g 

# object files

OBJS=  $(STARTUP) main.o 
OBJS+= stm32f10x_gpio.o
OBJS+= stm32f10x_rcc.o
OBJS+= stm32f10x_usart.o
OBJS+= stm32f10x_tim.o
OBJS+= stm32f10x_adc.o
OBJS+= stm32f10x_spi.o
OBJS+= uart.o
OBJS+= misc.o
OBJS+= interrupts.o 
OBJS+= fft.o 
OBJS+= guitar_tuner.o 
OBJS+= ssd1306.o 
OBJS+= delay.o 
OBJS+= peripheral_setup.o 


# include common make file

include $(TEMPLATEROOT)/Makefile.common


