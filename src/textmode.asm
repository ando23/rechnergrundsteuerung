; Copyright 2022 Andreas Herzig
; Licence: MIT
%line 2003

KTEXTMEM	equ	0xB8000


clrscr:
	push eax
;	push edx
	push edi
	mov edi, KTEXTMEM
	mov ecx, 80*25
	mov ah, byte [cursor_col]
	mov al, 0x20
	rep stosw
	pop edi
;	pop edx
	pop eax
	ret

disable_cursor:
	pushf
	push eax
	push edx
 
	mov dx, 0x3D4
	mov al, 0xA	; low cursor shape register
	out dx, al
 
	inc dx
	mov al, 0x20	; bits 6-7 unused, bit 5 disables the cursor, bits 0-4 control the cursor shape
	out dx, al
 
	pop edx
	pop eax
	popf
	ret


cursor_x:	dw 0
cursor_y:	dw 0
cursor_col:	db 0x17


; Output a single character
;	input	al	
;	clobbers	al
putc:
	; NL auswerten
	cmp al, 10
	jne .step2
		
	mov word [cursor_x], 0
	
	cmp word [cursor_y], 24
	jl .nlx
	call scrollup
	ret
.nlx:
	inc word [cursor_y]
	ret

.step2:
	; CR auswerten
	cmp al, 13
	jne .step3
	
	mov word [cursor_x], 0
	ret

.step3:
	; alles kleiner Space wird ignoriert
	cmp al, 32
	jge .step4
	
	ret

.step4:
	; Ausgabe
	push edi
	push ebx
	
	mov byte ah, [cursor_col]
	
	mov edi, KTEXTMEM
	xor ebx, ebx
	mov bx, word [cursor_y]
	imul ebx, 160
	add edi, ebx
	xor ebx, ebx
	mov bx, word [cursor_x]
	add edi, ebx
	add edi, ebx
		
	mov word [edi], ax
	
	inc bx
;	cmp bx, 80
;	jle .done
	
	mov word [cursor_x], bx
.done:
	pop ebx
	pop edi
	ret


; Output a string
;	input	eax
;	clobbers	eax
puts:
	push edi
	mov edi, eax
.loop:
	mov al, [edi]
	cmp al, 0
	je .end
	call putc
	inc edi
	jmp .loop
.end:
	pop edi
	ret


; Output a hex char without prefix
;	input	al
;	clobbers	al
puthexc:
	cmp al,10
	jl .p
	sub al, 10
	add al, 'A'
	call putc
	ret
.p:
	add al, '0'
	call putc
	ret


; Output value of EAX as hex with prefix '0x'
;	input	eax
putl:
	push ebx
	push eax
	mov al, '0'
	call putc
	mov al, 'x'
	call putc
	pop ebx
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call puthexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call puthexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call puthexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call puthexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call puthexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call puthexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call puthexc
	
	rol ebx,4
	mov al, bl
	and al, 0xf
	call puthexc
	
	pop ebx
	ret
	
; Output eax as decimal (uses stack as temp buffer)
;	input	eax
; 	clobbers	eax
putld:
	push ebx
	push edx
	push ebp
	
	; Eigenen Stack vorbereiten mit Platz für 11 Bytes
	mov ebp, esp
	sub esp, 16
	xor edx, edx

	; Abbruch-Marker auf Stack
	dec ebp
	mov [ebp], byte 0
	
	; Sonderfall 0
	cmp eax, 0
	je .zero
	
	; min 0
	; max 4.294.967.295	(10 Stellen)
		
.loop:
	;div ebx	; eax := (EDX:EAX / EBX), edx := (EDX:EAX % EBX)
	mov edx, 0
	mov ebx, 10
	div ebx
	
	push eax
	
	mov al, dl
	add al, '0'
	dec ebp
	mov [ebp], al
	;call putc
	
	pop eax

	cmp eax, 0
	je .print_stack
	
	jmp .loop
	
.print_stack:
	mov al, byte [ebp]
	inc ebp
	cmp al, byte 0
	je .end
	call putc
	jmp .print_stack
	
.zero:
	dec ebp
	mov [ebp], byte '0'
	jmp .print_stack
.end:
	add esp, 16
	pop ebp
	pop edx
	pop ebx
	ret


; Scroll the entire screen up one line and fill the bottom one with blanks
;	input	none
;	clobbers	none
scrollup:
	push eax
	push ecx
	push esi
	push edi
	mov edi, KTEXTMEM
	mov esi, KTEXTMEM + 160
	mov ecx, 80*24
	rep movsw
	mov edi, KTEXTMEM + 160*24
	mov ecx, 80
	mov ah, byte [cursor_col]
	mov al, 0x20
	rep stosw
	pop edi
	pop esi
	pop ecx
	pop eax
	ret


; Display formatted text / non-standard makros!
;	input	eax	Formatstring
;			%d	u32	dez
;			%w	u16	dez
;			%b	u8	dez
;			%D	u32	hex
;			%W	u16	hex
;			%B	u8	hex
;			%p	u32	hex (pointer)
;			%s	string
;	clobbers	eax
kprintf:
	push edi
	push ebp
	push ebx
	mov ebx, esp
	add ebx, 12
	mov ebp, ebx
	
	mov edi, eax
.loop:
	mov al, [edi]
	cmp al, 0
	je .end
	
	cmp al, '%'
	je .makro

.disp:
	call putc
.next:
	inc edi
	jmp .loop

.makro:
	inc edi
	mov al, [edi]
	cmp al, 0
	je .end
	cmp al, '%'
	je .disp
	cmp al, 'd'
	je .disp_u32
	cmp al, 'b'
	je .disp_u8
	cmp al, 'p'
	je .disp_p
	jmp .next

.disp_u8:
	push eax
	add ebp, 1
	mov eax, 0
	mov al, byte [ebp]
	call putld
	pop eax
	jmp .next

.disp_u32:
	push eax
	add ebp, 4
	mov eax, [ebp]
	call putld
	pop eax
	jmp .next
	
.disp_p:
	push eax
	add ebp, 4
	mov eax, [ebp]
	call putl
	pop eax
	jmp .next

.disp_s:
	push eax
	add ebp, 4
	mov eax, [ebp]
	call puts
	pop eax
	jmp .next
	
.end:
	pop ebx
	pop ebp
	pop edi
	ret
