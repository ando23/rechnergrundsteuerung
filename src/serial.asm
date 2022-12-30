; Copyright 2022 Andreas Herzig
; Licence: MIT
%line 3003

COM1	equ	0x3F8
COM2	equ	0x2F8
COM3	equ	0x3E8
COM4	equ	0x2E8

PORT	equ	0x3F8

init_kcom:
	push eax
	push edx
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
	jmp .done
 
.step2:
	; If serial is not faulty set it in normal operation mode
	; (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	OUTB PORT + 4, 0x0F
.done:
	pop edx
	pop eax
	ret

; Ein Zeichen ausgeben
; input:	al
; output:	none
; clobbers:	none	
kputc:
	push eax
.is_busy:
	; Prüfe ob bereit
	INB PORT+5, al
	and al, 0x20
	jz .is_busy
	pop eax
	push edx
	OUTB PORT, al
	pop edx
	ret

; String ausgeben
; input:	eax
; output:	none
; clobbers:	none	
kputs:
	push edi
	push eax
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
	pop eax
	pop edi
	ret

; String ausgeben
; input:	eax
; output:	none
; clobbers:	none	
kputs2:
	push edi
	push eax
	mov edi, eax
.loop:
	mov al, [edi]
	cmp al,0
	je .end
	call kputc
	inc edi
	jmp .loop
.end:
	pop eax
	pop edi
	ret


; Newline ausgeben
; input:	none
; output:	none
; clobbers:	none
kputnl:
	push eax
	mov al, 10
	call kputc
	pop eax
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

color2_on:
	push edx
	push eax
	OUTB PORT, 0x1b
	OUTB PORT, '['
	OUTB PORT, '3'
	OUTB PORT, '2'
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


; Hexdump of memory
; input:	edi	Adresse
; input:	ecx	Anzahl
; output:	none
; clobbers:	none
kputhd:
	push eax
	push ecx
	push edi
	
.loop:
	mov bl, [edi]
	mov al, bl
	and al, 0xf
	call kputhexc
		
	rol bl,4
	mov al, bl
	and al, 0xf
	call kputhexc

	mov al, 32
	call kputc
	
	inc edi
	dec ecx
	jnz .loop
	
	pop edi
	pop ecx
	pop eax
	ret


; Wert als Hex ausgeben
; input:	eax	32-Bit-Wert
; output:	none
; clobbers:	none
kputl:
	push eax
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
	pop eax
	ret


; Wert als Hex ausgeben
; input:	al	8-Bit-Wert
; output:	none
; clobbers:	none
kputb:
	push eax
	push ebx
	mov bl, al
	mov al, '0'
	call kputc
	mov al, 'x'
	call kputc
	
	rol bl,4
	mov al, bl
	and al, 0xf
	call kputhexc
	
	rol bl,4
	mov al, bl
	and al, 0xf
	call kputhexc

	pop ebx
	pop eax
	ret

kputb_dec_buf: db '0',0,0,0

; Wert dezimal ausgeben
; input:	al	8-Bit-Wert
; output:	none
; clobbers:	none
kputb_dec:
	push eax
	push ebx
	push ecx
	push edx
	push edi
	
	mov ebx, '0'
	mov [kputb_dec_buf], ebx

	mov ebx, eax

	xor eax, eax
	mov al, bl
	
	xor edx, edx	; edx := 0
	xor ecx, ecx	; ecx := 0
	mov ebx, 10	; 
	div ebx	; eax := (EDX:EAX / EBX), edx := (EDX:EAX % EBX)

	mov cl, dl
	
	xor edx, edx
	div ebx
	
	mov ch, dl
	
	mov edi, kputb_dec_buf
	
	cmp al, byte 0
	je .st2

	add al, byte '0'
	mov [edi], al
	inc edi
	
.st2:
	cmp ch, byte 0
	je .st3
	
	add ch, byte '0'
	mov [edi], ch
	inc edi

.st3:
	add cl, '0'
	mov [edi], cl
	inc edi
	mov [edi], byte 0
	
	mov eax, kputb_dec_buf
	call kputs2
		
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret
