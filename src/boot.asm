/* Entry point for Multiboot v1
*/


.set	MBALIGN,	0x00000001
.set	MEMINFO,	0x00000002
.set	MB1_GFX,	0x00000004
.set	MB1_ADR,	0x00010000
.set	FLAGS,	MBALIGN | MEMINFO 
.set	MAGIC,	0x1BADB002
.set	CHECKSUM,	-(MAGIC + FLAGS)
.set	KSTACK,	0x4000


.section .multiboot
multiboot_header:
	.long	MAGIC
	.long	FLAGS
	.long	CHECKSUM
	
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
/*	if !elf, then this must be filled and bit 16 of flags must be set
	.long	multiboot_header
	.long	start_of_kernel_image
	.long	end_of_kernel_image
	.long	end_of_kernel_bss
	.long	multiboot_entry
*/
	.long	0
	.long	640
	.long	480
	.long	32


.section .bss
.align 32
stack_bottom:
	.skip	KSTACK
stack_top:


.section .text

.global _start
.type _start, @function

.global kernel_main
.type kernel_main, @function

_start:
multiboot_entry:
	mov $stack_top, %esp
	pushl %ebx
	pushl %eax
	call kernel_main
	cli
.hang:	hlt
	jmp .hang
.end:

.size	_start, . - _start





