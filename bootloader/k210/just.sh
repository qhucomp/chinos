cargo build --target=riscv64imac-unknown-none-elf
rust-objcopy --binary-architecture=riscv64 ../target/riscv64imac-unknown-none-elf/debug/rustsbi-k210 --strip-all -O binary ../target/riscv64imac-unknown-none-elf/debug/rustsbi-k210.bin
