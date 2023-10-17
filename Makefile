
CC = gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -extra -m32 -march=i686
obj_files = kernel.o memory.h serial.o textmode.o
src_files = kernel.c memory.c serial.c textmode.c

all: out/kernel.bin

clean:
	rm -f out/kernel.bin
	rm -f obj/*.obj
	rm -f obj/*.o

obj/boot.o: $(wildcard src/*.asm)
	#mkdir out
	#nasm -Wall -O0 -f bin src/kernel.asm -o obj/boot.o
	nasm -Wall -O0 -f elf32 src/kernel.asm -o $@


obj/kernel.o: src/kernel.c
	$(CC) -c $< -o $@ $(CFLAGS)

obj/memory.o: src/memory.c
	$(CC) -c $< -o $@ $(CFLAGS)

obj/serial.o: src/serial.c
	$(CC) -c $< -o $@ $(CFLAGS)

obj/textmode.o: src/textmode.c
	$(CC) -c $< -o $@ $(CFLAGS)

out/kernel.bin: obj/boot.o obj/kernel.o obj/memory.o obj/serial.o obj/textmode.o
	gcc -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $< -m32 -march=i686
	#-lgcc

run: out/kernel.bin
	#set DBGTRIFAULT=-no-shutdown -no-reboot -d int
	qemu-system-i386 -kernel $< -m 16m  -serial stdio -rtc base=localtime

install-qemu:
	sudo apt install qemu-system

prepare:
	mkdir out
	mkdir obj

verify: out/kernel.bin
	grub-file --is-x86-multiboot out/kernel.bin
	echo $?
