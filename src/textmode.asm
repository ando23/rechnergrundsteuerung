; Copyright 2022 Andreas Herzig
; Licence: MIT

clrscr:
	push eax
	push edx
	push edi
	mov edi, 0xB8000
	mov ecx, 80*25
	mov ah, byte [cursor_col]
	mov al, 0x20
	rep stosw
	pop edi
	pop edx
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


;	input	al	
putc:
	; NL auswerten
	cmp al, 10
	jne .step2
		
	mov word [cursor_x], 0
	
	cmp word [cursor_y],25
	jl .nlx
	call scrollup
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
	
	mov edi, 0xB8000
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


;	input	eax	
puts:
	push edi
	mov edi, eax
.loop:
	mov al, [edi]
	cmp al,0
	je .end
	call putc
	inc edi
	jmp .loop
.end:
	pop edi
	ret


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


scrollup:
	push eax
	push edx
	push esi
	push edi
	mov edi, 0xB8000
	mov esi, 0xB8000+160
	mov ecx, 80*24
	rep movsw
	pop edi
	pop esi
	pop edx
	pop eax
	ret
	ret
