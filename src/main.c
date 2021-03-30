#include <bsp.h>
#include <dmac.h>
#include <ff.h>
#include <fpioa.h>
#include <plic.h>
#include <sdcard.h>
#include <stdio.h>
#include <sysctl.h>
#include <uarths.h>

int main(void) {
  sysctl_pll_set_freq(SYSCTL_PLL0, 800000000UL);

  plic_init();
  sysctl_enable_irq();

  while (1) printf("hello world\n");
  return 0;
}
