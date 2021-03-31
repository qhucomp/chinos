#include <bsp.h>
#include <dmac.h>
#include <ff.h>
#include <fpioa.h>
#include <plic.h>
#include <sdcard.h>
#include <stdio.h>
#include <sysctl.h>
#include <uarths.h>

#include "user_syscall.h"

int main(void) {
  // chino os init
  sysctl_pll_set_freq(SYSCTL_PLL0, 800000000UL);
  plic_init();
  sysctl_enable_irq();
  while (1)  //第二次调用会crash
    syscall(0, 1, 2, 3, 4, 5, 6);
  while (1)
    ;

  return 0;
}
