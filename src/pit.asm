<<<<<<< Updated upstream
; Copyright 2022 Andreas Herzig
; Licence: MIT
%line 7003

; Info: https://wiki.osdev.org/PIT

PIT_CHN0	equ 0x40
PIT_CHN1	equ 0x41
PIT_CHN2	equ 0x42
PIT_CMD	equ 0x43  

PIT_OS_FLAGS equ	0b0001_0110
;	  xx.. ....	channel
;	  ..xx ....	access mode, 01=lobyte only
;	  .... xxx.	operating mode, 3=square wave
;	  .... ...0	0=binary 1=BCD

; Initialise the Programmable Interrupt Timer
init_pit:
	pushad
	pushfd
	cli
	
	OUTB PIT_CMD, PIT_OS_FLAGS
	
	sti
	popfd
	popad
	ret

pit_counter:	dq 0

ISR_pit:
	push eax
	push ebx
	
	; do something
	mov eax, [pit_counter]
	inc eax
	mov [pit_counter], eax

	mov al, 0x20
	out 0x20, al	; Send the EOI to the PIC

	pop ebx
	pop eax
	iretd
	
pit_print:
	push eax
	
	mov eax, msg_pit_1
	call kputs
	mov eax, [pit_counter]
	call kputl
	call kputnl
	
	pop eax
	ret

msg_pit_1:	db "PIT: Zähler=",0
=======
; Copyright 2022 Andreas Herzig
; Licence: MIT
%line 7003

; Info: https://wiki.osdev.org/PIT

PIT_CHN0	equ 0x40
PIT_CHN1	equ 0x41
PIT_CHN2	equ 0x42
PIT_CMD	equ 0x43  

PIT_OS_FLAGS equ	0b0001_0110
;	  xx.. ....	channel
;	  ..xx ....	access mode, 01=lobyte only
;	  .... xxx.	operating mode, 3=square wave
;	  .... ...0	0=binary 1=BCD

; Initialise the Programmable Interrupt Timer
init_pit:
	pushad
	pushfd
	cli
	
	OUTB PIT_CMD, PIT_OS_FLAGS
	
	sti
	popfd
	popad
	ret

pit_counter:	dq 0

ISR_pit:
	push eax
	push ebx
	
	; do something
	mov eax, [pit_counter]
	inc eax
	mov [pit_counter], eax

	mov al, 0x20
	out 0x20, al	; Send the EOI to the PIC

	pop ebx
	pop eax
	iretd
	
pit_print:
	push eax
	
	mov eax, msg_pit_1
	call kputs
	mov eax, [pit_counter]
	call kputl
	call kputnl
	
	pop eax
	ret

msg_pit_1:	db "PIT: Zähler=",0
>>>>>>> Stashed changes
