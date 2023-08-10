<<<<<<< Updated upstream
; Copyright 2022 Andreas Herzig
; Licence: MIT

; Info: https://wiki.osdev.org/IDT
; Info: https://wiki.osdev.org/PIC

PIC1_COMMAND	equ 0x20
PIC1_DATA	equ 0x21
PIC2_COMMAND	equ 0xA0
PIC2_DATA	equ 0xA1

ICW1_INIT	equ 0x10
ICW1_ICW4	equ 0x01
ICW4_8086	equ 0x01

INT_OFFSET1	equ	0x20
INT_OFFSET2	equ	0x70

; clobbers	ax, dx, cx
RemapPIC:
	INB PIC1_DATA, al
	mov cl, al
	INB PIC2_DATA, al
	mov ch, al

	OUTB PIC1_COMMAND, ICW1_INIT | ICW1_ICW4
	OUTB PIC2_COMMAND, ICW1_INIT | ICW1_ICW4
	OUTB PIC1_DATA, INT_OFFSET1
	OUTB PIC2_DATA, INT_OFFSET2
	OUTB PIC1_DATA, 4	; ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	OUTB PIC2_DATA, 2	; ICW3: tell Slave PIC its cascade identity (0000 0010)

	OUTB PIC1_DATA, ICW4_8086
	OUTB PIC2_DATA, ICW4_8086

	OUTB PIC1_DATA, cl
	OUTB PIC2_DATA, ch
	
	ret
=======
; Copyright 2022 Andreas Herzig
; Licence: MIT

; Info: https://wiki.osdev.org/IDT
; Info: https://wiki.osdev.org/PIC

PIC1_COMMAND	equ 0x20
PIC1_DATA	equ 0x21
PIC2_COMMAND	equ 0xA0
PIC2_DATA	equ 0xA1

ICW1_INIT	equ 0x10
ICW1_ICW4	equ 0x01
ICW4_8086	equ 0x01

INT_OFFSET1	equ	0x20
INT_OFFSET2	equ	0x70

; clobbers	ax, dx, cx
RemapPIC:
	INB PIC1_DATA, al
	mov cl, al
	INB PIC2_DATA, al
	mov ch, al

	OUTB PIC1_COMMAND, ICW1_INIT | ICW1_ICW4
	OUTB PIC2_COMMAND, ICW1_INIT | ICW1_ICW4
	OUTB PIC1_DATA, INT_OFFSET1
	OUTB PIC2_DATA, INT_OFFSET2
	OUTB PIC1_DATA, 4	; ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	OUTB PIC2_DATA, 2	; ICW3: tell Slave PIC its cascade identity (0000 0010)

	OUTB PIC1_DATA, ICW4_8086
	OUTB PIC2_DATA, ICW4_8086

	OUTB PIC1_DATA, cl
	OUTB PIC2_DATA, ch
	
	ret
>>>>>>> Stashed changes
