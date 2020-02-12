#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include "uart.h"


// define buffers here.

int uart_open (USART_TypeDef* USARTx, uint32_t baud, uint32_t flags)
{
  USART_InitTypeDef USART_InitStructure; 
  GPIO_InitTypeDef GPIO_InitStructureTx; 
  GPIO_InitTypeDef GPIO_InitStructureRx;

  assert_param(IS_USART_123_PERIPH(USARTx));

  if (USARTx == USART1) {

    // Turn on clocks

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  |
			   RCC_APB2Periph_AFIO,
			   ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // Configure TX pin

    GPIO_InitStructureTx.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructureTx.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructureTx.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructureTx);

    // Configure RX pin

    GPIO_InitStructureRx.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructureRx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructureRx);

    // Configure the UART

    USART_StructInit(&USART_InitStructure); 
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_Mode  = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1,&USART_InitStructure); 
    USART_Cmd(USART1, ENABLE); 

    return 0;
  } 
}


int uart_close(USART_TypeDef* USARTx)
{
    assert_param(IS_USART_123_PERIPH(USARTx));
}

int uart_putc(int c, USART_TypeDef* USARTx)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
  USARTx->DR =  (c & 0xff);
  return 0;
}

int uart_getc (USART_TypeDef* USARTx)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
  return  USARTx->DR & 0xff;
}

void uart_puts(const char* str, USART_TypeDef* USARTx)
{
    while(*str) uart_putc(*str++, USARTx);
}

int uart_gets(char* buff, int len, USART_TypeDef* USARTx)
{
    if(len < 1) return 0;

    int c, i;

    i = 0;
    while(1)
    {
	// read char
	c = uart_getc(USARTx);
	if(!c) return -1;
	if((c == '\r') || (c == '\n')) break;

	if(c >= ' ' && i < len-1)
	{
	    buff[i++] = c;
	    uart_putc(c, USARTx);
	}
    }
    uart_puts("\n\r", USARTx);
    buff[i] = '\0';
    return 0;
}

// Send a decimal value as ascii character over UART
void uart_sendnumber(u32 x, USART_TypeDef* USARTx)
{
    char value[10]; //a temp array to hold results of conversion
    int i = 0; //loop index

    do
    {
	value[i++] = (char)(x % 10) + '0'; //convert integer to character
	x /= 10;
    } while(x);

    while(i)
    {
	uart_putc(value[--i], USARTx);
    }
}
