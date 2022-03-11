MB1_MAGIC	equ	0x1BADB002
MB1_MAGIC_RESP	equ	0x2BADB002
MB1_ALIGN	equ	1<<0
MB1_MEMORY	equ	1<<1
MB1_AOUT_KLUDGE	equ  1 << 16
MB1_FLAGS	equ	(MB1_ALIGN | MB1_MEMORY | MB1_AOUT_KLUDGE)
MB1_CHKSUM	equ	-(MB1_MAGIC + MB1_FLAGS)

MB1_FLAGS2	equ	0b1000000000000011
	;	  ...............x	ALIGN
	;	  ..............x.	MEMORY
	;	  x...............	AOUT_KLUDGE

align 4, db 0
multiboot1_header:
	dd	MB1_MAGIC
	dd	MB1_FLAGS
	dd	MB1_CHKSUM
	
	; Addr bei AOUT_KLUDGE
	dd	multiboot1_header	; header addr
	dd	_start	; load to
	dd	00	; ignored
	dd	00	; ignored
	dd	kernel_start	; starting point

	; Grub VGA
	dd	0
	dd	0
	dd	0
	dd	0
	
multiboot1_infoptr:
	dq	0



