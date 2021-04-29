#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include "include/printk.h"
#include "include/uart.h"
int printk(const char *fmt,...) {
    int ret;
    va_list ap;
    va_start(ap,fmt);
    ret = vfprintk(fmt,ap);
    va_end(ap);
    return ret;
}
/**
 * @brief 将数字转换为字符串
 * 
 * @param[in] val 被转换的数字的值
 * 
 * @param[in] buf 缓冲数组
 * 
 * @param[in] radix 转换为radix进制
 */
static char *itoa(int64_t val, char *buf, unsigned radix)
{
    char   *p;             
    char   *firstdig;      
    char   temp;           
    unsigned   digval;     
    p = buf;
    if(val < 0)
    {
        *p++ = '-';
        val = -val;
    }
    firstdig = p; 
    do{
        digval = (unsigned)(val % radix);
        val /= radix;
       
        if  (digval > 9)
            *p++ = (char)(digval - 10 + 'a'); 
        else
            *p++ = (char)(digval + '0');      
    }while(val > 0);
   
    *p-- = '\0';         
    do{
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;
        --p;
        ++firstdig;        
    }while(firstdig < p);  
    return buf;
}

int vfprintk(const char *fmt,va_list ap) {
    char buf[21];
    for(size_t i = 0;fmt[i] != '\0';i++) {
        switch(fmt[i]) {
            case '%':
                i++;
                switch(fmt[i]) {
                    case 'd':
                        itoa(va_arg(ap,int),buf,10);
                        int len;
                        for(len = 0;buf[len] != 0;len++)
                            uart_send(buf[len]);
                        break;
                    case 'l':
                        i++;
                        if (fmt[i] == 'd') {
                            itoa(va_arg(ap,int),buf,10);
                            int len;
                            for(len = 0;buf[len] != 0;len++)
                                uart_send(buf[len]);
                        }
                        break;
                    case 'p':
                    case 'x':
                        itoa(va_arg(ap,uintptr_t),buf,16);
                        for(size_t len = 0;buf[len] != 0;len++)
                            uart_send(buf[len]);
                        break;
                    case 'X':
                        itoa(va_arg(ap,uintptr_t),buf,16);
                        uart_send('0');
                        uart_send('x');
                        for(size_t len = 0;buf[len] != 0;len++)
                            uart_send(buf[len]);
                        break;
                    case 'o':
                        itoa(va_arg(ap,int),buf,8);
                        for(size_t len = 0;buf[len] != 0;len++)
                            uart_send(buf[len]);
                        break;
                    case 'c':
                        uart_send((char)va_arg(ap,int));
                        break;
                    case 's': {
                        char *s = va_arg(ap,char *);
                        for(size_t len = 0;s[len] != 0;len++)
                            uart_send(s[len]);
                        break;
                    }
                    case '%':
                        uart_send('%');
                        break;
                }
                break;
            default:
                uart_send(fmt[i]);
            
        }
    }
    return 0;
}