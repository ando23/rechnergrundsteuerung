; Copyright 2022 Andreas Herzig
; Licence: MIT

; Info: https://wiki.osdev.org/IDT
; Info: https://wiki.osdev.org/Interrupts_tutorial

align 4, db 0
IDT_Descriptor:
	dw	(256 * 8) - 1	; For limit storage
	dq	IDT_Table	; For base storage

align 4, db 0
IDT_Table:
	times 8 * 256 db 0
	
struc idte_t
.offsetl:	resw	1
.segsel:	resw	1
.reserved:	resb	1
.flags:	resb	1
.offseth:	resw	1
endstruc	


init_IDT:
	push edi
	push eax
	push ebx
	
	mov eax, msg_dbg1
	call kputs
	
	mov edi, IDT_Table
	mov ebx, Default_ISR
	mov eax, 0
.loop:
	;call kputl
	call init_IDTE
	add edi, 8
	inc eax
	cmp eax, 256
	jl .loop
	
	mov eax, msg_dbg2
	call kputs
	
	; remapPIC
	call RemapPIC
	
	OUTB 0x21, 0xfd
	OUTB 0xa1, 0xff

.init_keyboard:
	mov eax, IDT_Table + 0x21 * 8
	mov edi, eax
	mov eax, 0x21
	mov ebx, ISR_Keyb
	call init_IDTE
	
.make_test_isr:
	mov eax, IDT_Table + 49 * 8
	mov edi, eax
	mov eax, 49
	mov ebx, Test_ISR
	call init_IDTE
	
	mov eax, msg_dbg2
	call kputs
	
.init_pit:
	mov eax, IDT_Table + 0x20 * 8
	mov edi, eax
	mov eax, 0x20
	mov ebx, ISR_pit
	call init_IDTE
	mov eax, IDT_Table + 0x28 * 8
	mov edi, eax
	mov eax, 0x28
	mov ebx, ISR_pit
	call init_IDTE

.done:
	mov eax, msg_dbg3
	call kputs
	lidt [IDT_Descriptor]
	
	mov eax, msg_dbg4
	call kputs
	
	pop ebx
	pop eax
	pop edi
	ret



;	input	edi	pointer to IDT_entry
;	input	eax	idt#
;	input	ebx	handler
;	clobbers	ebx
init_IDTE:
	push eax
	push ebx
	;shl eax, 3
	
	; Segment Selector
	mov word [edi + idte_t.segsel], LINEAR_CODE_SELECTOR	; Ring0-Code
	mov byte [edi + idte_t.reserved], 0x00	; reserved
	
	; Offset
	mov word [edi + idte_t.offsetl], bx
	shr ebx,16
	mov word [edi + idte_t.offseth], bx
	
	; Flags
	mov byte [edi + idte_t.flags],	0b1000_1110
	;	  .... xxxx	Gate Type		1110 = 32bit Interrupt Gate
	;	  ...0 ....	reserviert
	;	  .xx. ....	DPL
	;	  1... ....	Present-Flag
	
	pop ebx
	pop eax
	ret
	

align 4
Default_ISR:
	pushad
	cld

	push eax

	mov ax, LINEAR_DATA_SELECTOR
	mov gs, ax

	pop eax
	call kputl

	mov eax, int_msg
	call kputs
	
	popad
	iret
	
align 4
Test_ISR:
	pushad
	cld
	
	mov ax, LINEAR_DATA_SELECTOR
	mov gs, ax
	mov dword [gs:0xb8000], 'T E '
	mov dword [gs:0xb8004], 'S T '
	
	mov eax, msg_int_test
	call kputs
	
	popad
	iret
	
align 4
ISR_Keyb:
	push eax
	cld
	in al, 60h
	
	mov eax, int_keyb_msg
	call puts

	mov al, 20h
	out 20h, al
	;OUTB 0x20, 0x20
	;OUTB 0xa0, 0x20
	pop eax
	iret


; https://wiki.osdev.org/Exception
align 4
exc_0d_handler:
    push gs
	mov ax, LINEAR_DATA_SELECTOR	;ZEROBASED_DATA_SELECTOR
	mov gs, ax	
	mov word [gs:0xb8000],'D '
	;; D in the top-left corner means we're handling
	;;  a GPF exception right ATM.

	;; your 'normal' handler comes here
	pushad
	push ds
	push es
	mov ax, LINEAR_DATA_SELECTOR	; KERNEL_DATA_SELECTOR
	mov ds,ax
	mov es,ax

	;call gpfExcHandler

	pop es
	pop ds
	popad

	mov dword [gs:0xb8000],'  D-'
	;; the 'D' moved one character to the right, letting
	;; us know that the exception has been handled properly
	;; and that normal operations continues.
	pop gs
	iret


int_msg:	db "Hallo Interrupt!", 10, 0
int_keyb_msg:	db "Hallo Keyboard!", 10, 0
msg_int_test:	db "Test-Interrupt ist hier.", 10, 0

