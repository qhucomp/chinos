obj := \
kernel/main.o \
kernel/entry_k210.o \
kernel/uart.o \
kernel/trap_entry.o \
kernel/trap.o \
kernel/syscalls.o \
kernel/riscv.o \
kernel/printk.o \
kernel/print_logo.o \
kernel/kmalloc.o \
kernel/string.o \
kernel/kernel_thread.o \
kernel/idle.o \
kernel/sysctl.o \
kernel/task.o \
kernel/timer.o \
kernel/plic.o \
kernel/thread_test.o \
kernel/scheduler.o \
kernel/spi.o \
kernel/misc.o \
kernel/fpioa.o \
kernel/gpiohs.o \
kernel/sdcard.o \
kernel/diskio.o \
kernel/fat32.o \
kernel/vfs.o \
kernel/sleep.o \
kernel/dmac.o \
kernel/user.o \
kernel/shell.o \
kernel/init.o \
kernel/elf_parse.o
ifeq ($(OS),Windows_NT)
	r := $(shell cd bootloader/k210 && sh just.sh)
	r := $(shell cp bootloader/target/riscv64imac-unknown-none-elf/debug/rustsbi-k210.bin .)
	command_path := toolchain/bin/
else
	command_path := 
	r := $(shell export PATH=$PATH:/opt/kendryte-toolchain/bin)
endif
prefix := $(command_path)riscv64-unknown-elf-
CC := $(prefix)gcc
LD := $(prefix)ld
OBJCOPY := $(prefix)objcopy
OBJDUMP := $(prefix)objdump
CFLAGS := -mcmodel=medany -Wall -Werror -O0 -fno-omit-frame-pointer -MD -fno-common -mno-relax -fno-stack-protector -nostdlib -ffreestanding
lds := kernel/k210.lds
DD := $(command_path)dd
all: $(obj)


	$(LD) -o kernel/kernel.elf -T $(lds) $(obj)
	$(OBJCOPY) kernel/kernel.elf --strip-all -O binary kernel.bin
	$(OBJDUMP) -d kernel/kernel.elf > kernel.asm
	$(DD) if=kernel.bin of=rustsbi-k210.bin bs=128k seek=1
	cp rustsbi-k210.bin k210.bin
	rm kernel.bin
entry_k210:kernel/entry_k210.S
	$(CC) $(CFLAGS) -c entry_k210.S
clean:
	rm kernel/*.d kernel/*.o kernel/kernel.elf
upload:all
	$(command_path)kflash_py k210.bin
clean_cache:
	rm bootloader/target -rf