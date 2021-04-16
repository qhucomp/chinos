#ifndef __UART_H
#define __UART_H

void uart_send(char ch);
int uart_receive(void);
#define EOF ((int)-1)

#endif