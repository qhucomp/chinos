#include "include/uart.h"
#include "include/platform.h"
#include <stdint.h>
#define reg(r) ((volatile uint32_t *)(UARTHS_BASE_ADDR + r))

#define write_reg(r,c)  (*(reg(r))=c)
#define read_reg(r)     (*(reg(r)))
#define TXDATA  0
#define RXDATA  4
#define DATA_IS_OK(data) ((data) & (1 << 31) == 0)
#define DATA_IS_FINSHED(data) ((data) & (1 << 31))
void uart_send(char ch) {
    while(DATA_IS_FINSHED(read_reg(TXDATA)))
      continue;
    write_reg(TXDATA,ch);
}

int uart_receive(void) {
    uint32_t data = read_reg(RXDATA);
    if (DATA_IS_OK(data))
        return data & 0xff;
    else
        return EOF;
}