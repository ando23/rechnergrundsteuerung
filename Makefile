
all: out/kernel.bin

out/kernel.bin: $(wildcard src/*.asm)
	#mkdir out
	nasm -Wall -O0 -f bin src/kernel.asm -o out/kernel.bin

clean:
	rm -f out/kernel.bin

run: out/kernel.bin
	#set QEMU="C:\Tools\qemu\qemu-system-i386.exe"
	#set DBGTRIFAULT=-no-shutdown -no-reboot -d int
	qemu-system-i386 -kernel out/kernel.bin -m 16m  -serial stdio -rtc base=localtime

