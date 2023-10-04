
all: out/kernel.bin

out/kernel.bin: $(wildcard src/*.asm)
	#mkdir out
	nasm -Wall -O0 -f bin src/kernel.asm -o out/kernel.bin

obj/boot.o: $(wildcard src/*.asm)
	#mkdir out
	#nasm -Wall -O0 -f bin src/kernel.asm -o obj/boot.o
	nasm -Wall -O0 -f elf32 src/kernel.asm -o obj/boot.o
	
obj/test.o: src/test.c
	gcc -c src/test.c -o obj/test.o -std=gnu99 -ffreestanding -O2 -Wall -extra -m32 -march=i686

link: obj/boot.o obj/test.o
	gcc -T linker.ld -o out/kernel2.bin -ffreestanding -O2 -nostdlib obj/boot.o obj/test.o -m32 -march=i686
	#-lgcc 

clean:
	rm -f out/kernel.bin
	rm -f obj/*.obj
	rm -f obj/*.o

run: out/kernel.bin
	#set QEMU="C:\Tools\qemu\qemu-system-i386.exe"
	#set DBGTRIFAULT=-no-shutdown -no-reboot -d int
	qemu-system-i386 -kernel out/kernel.bin -m 16m  -serial stdio -rtc base=localtime

install-qemu:
	sudo apt install qemu-system

verify: out/kernel.bin
	grub-file --is-x86-multiboot out/kernel.bin
	echo $?
