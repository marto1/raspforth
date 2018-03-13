
ARMGNU ?= arm-none-eabi
COPS = -Wall -O3 -nostdlib -nostartfiles -ffreestanding 

gcc : forth.hex forth.bin

all : gcc clang

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

forth.o : forth.c
	$(ARMGNU)-gcc $(COPS) -c forth.c -o forth.o

forth.elf : memmap vectors.o forth.o 
	$(ARMGNU)-ld vectors.o forth.o -T memmap -o forth.elf
	$(ARMGNU)-objdump -D forth.elf > forth.list

forth.bin : forth.elf
	$(ARMGNU)-objcopy forth.elf -O binary forth.bin

forth.hex : forth.elf
	$(ARMGNU)-objcopy forth.elf -O ihex forth.hex







LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : forth.clang.hex forth.clang.bin


forth.clang.bc : forth.c
	clang $(LOPS) -c forth.c -o forth.clang.bc

forth.clang.opt.elf : memmap vectors.o forth.clang.bc
	opt $(OOPS) forth.clang.bc -o forth.clang.opt.bc
	llc $(LLCOPS) forth.clang.opt.bc -o forth.clang.opt.s
	$(ARMGNU)-as forth.clang.opt.s -o forth.clang.opt.o
	$(ARMGNU)-ld -o forth.clang.opt.elf -T memmap vectors.o forth.clang.opt.o
	$(ARMGNU)-objdump -D forth.clang.opt.elf > forth.clang.opt.list

forth.clang.hex : forth.clang.opt.elf
	$(ARMGNU)-objcopy forth.clang.opt.elf forth.clang.hex -O ihex

forth.clang.bin : forth.clang.opt.elf
	$(ARMGNU)-objcopy forth.clang.opt.elf forth.clang.bin -O binary





