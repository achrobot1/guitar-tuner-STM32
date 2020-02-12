#ifndef _UART_H
#define _UART_H

int uart_open(USART_TypeDef* USARTx, uint32_t baud, uint32_t flags);
int uart_close(USART_TypeDef* USARTx);
int uart_putc(int c, USART_TypeDef* USARTx);
int uart_getc(USART_TypeDef* USARTx);
void uart_puts(const char* str, USART_TypeDef* USARTx);
int uart_gets(char* buff, int len, USART_TypeDef* USARTx);
void uart_sendnumber(u32 x, USART_TypeDef* USARTx);

#endif
