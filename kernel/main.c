#include "include/riscv.h"
#include "include/uart.h"
#include "include/platform.h"
#define ECALL(__num, __a0, __a1, __a2)                                       \
  ({                                                                         \
    register unsigned long a0 asm("a0") = (unsigned long)(__a0);             \
    register unsigned long a1 asm("a1") = (unsigned long)(__a1);             \
    register unsigned long a2 asm("a2") = (unsigned long)(__a2);             \
    register unsigned long a7 asm("a7") = (unsigned long)(__num);            \
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory"); \
    a0;                                                                      \
  })
void putchar(char c) {
  ECALL(1,c,0,0);
}
char *itoa(int val, char *buf, unsigned radix)
{
    char   *p;             
    char   *firstdig;      
    char   temp;           
    unsigned   digval;     
    p = buf;
    if(val <0)
    {
        *p++ = '-';
        val = (unsigned long)(-(long)val);
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
int main(void) {
    char *start_info = "start kernel...\n";
    for(int i = 0;i < 16;i++)
      uart_send(start_info[i]);

    while(1) {
      int ch =   uart_receive();
      if (ch != EOF) { 
          uart_send(ch);
      }
    }
    return 0;
}