; Copyright 2022 Andreas Herzig
; Licence: MIT

[bits 32]
[org 0x100000]
[global _start]

KSTACK_SIZE	equ	4096

section .text
_start:
	xor    eax, eax	; Clear eax and ebx in the event
	xor    ebx, ebx	; we are not loaded by GRUB.
	jmp    kernel_start	; Jump over the multiboot header

%include "src\multiboot1.asm"

kernel_start:
	cli
	cld
	
	mov esp, kstack_top
	push 0
	popf
		

; Check Multiboot
	cmp eax, MB1_MAGIC_RESP
	jne kernel_fail

	mov [multiboot1_infoptr], ebx


	call disable_cursor
	call clrscr


; OK, we have Multiboot!
	mov eax, msg_hello
	call puts
	mov byte [cursor_col], 0x07


kinit_ser:
	call init_kcom
	mov eax, msg_ser
	call kputs

kinit_cpu:
	mov eax, msg_cpu
	call kputs
	
	call check_cpu
	
	call init_sse


kinit_pmode:	; Enter Protected Mode

	mov eax, msg_pmode
	call kputs
	call enter_pmode
	
kernel_interrupts:
	mov eax, msg_ints
	call kputs	; init interrupts
	call init_IDT

kinit_pit:
	;mov eax, msg_pit
	;call kputs
	;call init_pit

kinit_done:
	mov eax, msg_init_done
	call kputs
	
	; Interrupts töten, Karl!
	sti
	
	mov eax, msg_dbg1
	call kputs
	
	int 49

	;jmp kernel_start_external_main
	;jmp kernel_loop
	;jmp kernel_shutdown

kernel_demo:
	mov edi, 0xB8000 + 20 * 160
	mov bx, 0x4041
	mov cx, 0x3042
	mov eax, 0
	
	.loop1:
		mov word [edi + eax], bx
		add ax, 2
		cmp ax, 160
		jne .loop1
		mov ax, 0
	
	.loop2:
		mov word [edi + eax], cx
		add ax, 2
		cmp ax, 160
		jne .loop2
		mov ax, 0
		
	jmp .loop1

kernel_loop:
	nop
	nop
	nop
	nop
	jmp kernel_loop

kernel_shutdown:
	mov eax, msg_end
	call kputs
	jmp kernel_halt


kernel_fail:
	; hier Bluescreen of death :-)
	mov word [0xB8000], 0x4020
	mov word [0xB8002], 0x4046
	mov word [0xB8004], 0x4061
	mov word [0xB8006], 0x4069
	mov word [0xB8008], 0x406c
	mov word [0xB800A], 0x4020
	jmp kernel_halt


kernel_halt:
	cli
	hlt
	jmp kernel_halt


kernel_end:

%include "src/cpu.asm"
%include "src/textmode.asm"
%include "src/serial.asm"
%include "src/pic.asm"
%include "src/memory.asm"
%include "src/interrupts.asm"
%include "src/pit.asm"


section .bss
align 32
kstack:
	resb KSTACK_SIZE
kstack_top:

section .data
%define NL 10
msg_hello:	db NL, " [SYS/A v0.1] ", NL, NL, 0
%define NL 13,10
msg_ser:	db "[",0x1b,"[33mSYS/A",0x1b,"[36m] Starte ...", NL, 0
msg_cpu:	db "CPU ...", NL, 0
msg_pmode:	db "Wechsel in den Protected Mode ...", NL, 0
msg_ints:	db "Richte Int-Handler ein ...", NL, 0
msg_pit:	db "Initialisiere Timer ...", NL, 0
msg_init_done:	db "Initialisierung vollstaendig. Viel Spass!", NL, NL, 0
msg_end:	db "Fahre herunter.", NL, 0
msg_dbg1:	db "  ",0x1b,"[31;1mDEBUG #1", NL, 0
msg_dbg2:	db "  ",0x1b,"[31;1mDEBUG #2", NL, 0
msg_dbg3:	db "  ",0x1b,"[31;1mDEBUG #3", NL, 0
msg_dbg4:	db "  ",0x1b,"[31;1mDEBUG #4", NL, 0
msg_dbg5:	db "  ",0x1b,"[31;1mDEBUG #5", NL, 0
