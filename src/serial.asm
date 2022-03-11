; Copyright 2022 Andreas Herzig
; Licence: MIT

COM1	equ	0x3F8
COM2	equ	0x2F8
COM3	equ	0x3E8
COM4	equ	0x2E8

PORT	equ	0x3F8

init_kcom:
	OUTB (PORT + 1), 0x00	; Disable all interrupts
	OUTB (PORT + 3), 0x80	; Enable DLAB (set baud rate divisor)
	OUTB (PORT + 0), 0x03	; Set divisor to 3 (lo byte) 38400 baud
	OUTB (PORT + 1), 0x00	;                  (hi byte)
	OUTB (PORT + 3), 0x03	; 8 bits, no parity, one stop bit
	OUTB (PORT + 2), 0xC7	; Enable FIFO, clear them, with 14-byte threshold
	OUTB (PORT + 4), 0x0B	; IRQs enabled, RTS/DSR set
	OUTB (PORT + 4), 0x1E	; Set in loopback mode, test the serial chip
	OUTB (PORT + 0), 0xAE	; Test serial chip (send byte 0xAE and check if serial returns same byte)

	; Check if serial is faulty (i.e: not same byte as sent)
	INB (PORT), al
	cmp al, 0xAE
	je .step2
	
	;FIXME Log error
	ret
 
.step2:
	; If serial is not faulty set it in normal operation mode
	; (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	OUTB PORT + 4, 0x0F
	ret

kputc:
	push eax
.is_busy:
	; Prüfe ob bereit
	INB PORT+5, al
	and al, 0x20
	jz .is_busy
	pop eax
	OUTB PORT, al
	ret
	
kputs:
	push edi
	mov edi, eax
	call color_on	
.loop:
	mov al, [edi]
	cmp al,0
	je .end
	call kputc
	inc edi
	jmp .loop
.end:
	call color_off
	pop edi
	ret

color_on:
	push edx
	push eax
	OUTB PORT, 0x1b
	OUTB PORT, '['
	OUTB PORT, '3'
	OUTB PORT, '6'
	OUTB PORT, 'm'
	pop eax
	pop edx
	ret
color_off:
	push edx
	push eax
	OUTB PORT, 0x1b
	OUTB PORT, '['
	OUTB PORT, '0'
	OUTB PORT, 'm'
	pop eax
	pop edx
	ret

;	input	al
;	clobbers	al
kputhexc:
	cmp al,10
	jl .p
	sub al, 10
	add al, 'A'
	call kputc
	ret
.p:
	add al, '0'
	call kputc
	ret

	
;	input	eax
kputl:
	push ebx
	push eax
	mov al, '0'
	call kputc
	mov al, 'x'
	call kputc
	pop ebx
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call kputhexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call kputhexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call kputhexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call kputhexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call kputhexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call kputhexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call kputhexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call kputhexc
	
	pop ebx
	ret

