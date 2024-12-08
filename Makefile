
SRCDIR=src
OBJDIR=obj

CC = gcc
#  -fsanitize=undefined
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -march=i686 -masm=intel 

ASM=as
ASMFLAGS=--32

LINK=ld
LDFLAGS=-T linker.ld -nostdlib --gc-sections -m elf_i386

src_files = kernel.c memory.c string.c serial.c textmode.c cpu.c cpuid.c idt.c pci.c pit.c rtc.c gfx.c
obj_files = $(OBJDIR)/kernel.o $(OBJDIR)/memory.o $(OBJDIR)/string.o $(OBJDIR)/serial.o $(OBJDIR)/textmode.o $(OBJDIR)/cpu.o $(OBJDIR)/cpuid.o $(OBJDIR)/idt.o $(OBJDIR)/pci.o $(OBJDIR)/pit.o $(OBJDIR)/rtc.o $(OBJDIR)/gfx.o

#set DBGTRIFAULT=-no-shutdown -no-reboot -d int
EMUFLAGS = -m 4m -serial stdio -rtc base=localtime
## -device rtl8139
EMUVGAFLAGS = -vga std
#EMUVGAFLAGS = -device bochs-display	### gibt ein zusätzliches!
#EMUVGAFLAGS = -device virtio-vga
#,virgl=on,max_outputs=2

.PHONY: all clean install-qemu run verify prepare
.SUFFIXES: .o .c .asm

all: out/kernel.bin

clean:
	rm -f out/kernel.bin
	rm -f obj/*.obj
	rm -f obj/*.o
	
# Assemble
$(OBJDIR)/boot.o : $(SRCDIR)/boot.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

#obj/boot.o: $(wildcard src/*.asm)
#	#mkdir out
#	#nasm -Wall -O0 -f bin src/kernel.asm -o obj/boot.o
#	nasm -Wall -O0 -f elf32 src/kernel.asm -o $@


#.c.o:
#	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

# Compile
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
# Link
out/kernel.bin: obj/boot.o $(obj_files)
	#gcc -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $< -m32 -march=i686
	#-lgcc
	# Link
	$(LINK) $(LDFLAGS) -o $@ $^

run: out/kernel.bin
	qemu-system-i386 -kernel $< $(EMUFLAGS) $(EMUVGAFLAGS)

install-qemu:
	sudo apt install qemu-system

prepare:
	mkdir out
	mkdir obj

verify: out/kernel.bin
	grub-file --is-x86-multiboot out/kernel.bin
	echo $?
