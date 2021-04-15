obj := kernel/main.o kernel/entry_k210.o

command_path :=  
prefix := $(command_path)riscv64-unknown-elf-
CC := $(prefix)gcc
LD := $(prefix)ld
OBJCOPY := $(prefix)objcopy
CFLAGS := -mcmodel=medany -Wall -Werror -O -fno-omit-frame-pointer -MD -ffreestanding -fno-common -nostdlib -mno-relax -fno-stack-protector
lds := kernel/k210.lds
DD := $(command_path)dd
build: $(obj)
	cd bootloader/k210 && just
	cp bootloader/target/riscv64imac-unknown-none-elf/debug/rustsbi-k210.bin .
	$(LD) -o kernel/kernel.elf -T $(lds) $(obj)
	$(OBJCOPY) kernel/kernel.elf --strip-all -O binary kernel.bin
	$(DD) if=kernel.bin of=rustsbi-k210.bin bs=128k seek=1
	mv rustsbi-k210.bin k210.bin
	rm kernel.bin
entry_k210:kernel/entry_k210.S
	$(CC) $(CFLAGS) -c entry_k210.S