#define ECALL(__num, __a0, __a1, __a2)                                       \
  ({                                                                         \
    register unsigned long a0 asm("a0") = (unsigned long)(__a0);             \
    register unsigned long a1 asm("a1") = (unsigned long)(__a1);             \
    register unsigned long a2 asm("a2") = (unsigned long)(__a2);             \
    register unsigned long a7 asm("a7") = (unsigned long)(__num);            \
    asm volatile("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7) : "memory"); \
    a0;                                                                      \
  })
extern char *_sbss;
extern char *_ebss;
void putchar(char c) {
    ECALL(1,c,0,0);
}
int main(void) {
    char *kernel_test = "Hello world\n";
    int ret = 0;
    for(int i = ret;i < 12;i++)
        putchar(kernel_test[i]);
    while(1);
    return 0;
}