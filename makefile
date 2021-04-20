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
kernel/user_thread.o \
kernel/user.o \


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
CFLAGS := -mcmodel=medany -Wall -Werror -O -fno-omit-frame-pointer -MD -ffreestanding -fno-common -nostdlib -mno-relax -fno-stack-protector
lds := kernel/k210.lds
DD := $(command_path)dd
all: $(obj)


	$(LD) -o kernel/kernel.elf -T $(lds) $(obj)
	$(OBJCOPY) kernel/kernel.elf --strip-all -O binary kernel.bin
	$(DD) if=kernel.bin of=rustsbi-k210.bin bs=128k seek=1
	cp rustsbi-k210.bin k210.bin
	rm kernel.bin
entry_k210:kernel/entry_k210.S
	$(CC) $(CFLAGS) -c entry_k210.S
clean:
	rm kernel/*.d kernel/*.o kernel/kernel.elf
upload:
	$(command_path)kflash_py k210.bin
clean_cache:
	rm bootloader/target -rf