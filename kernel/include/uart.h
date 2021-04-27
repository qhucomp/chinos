#ifndef __UART_H
#define __UART_H
/**
 * @brief 通过串口发送数据
 * 
 * @param[in] ch 要发送的数据
 */
void uart_send(char ch);

/**
 * @brief 接收串口发送的数据
 * 
 * @return 返回串口发送来的数据
 */
int uart_receive(void);
#define EOF ((int)-1)

#endif