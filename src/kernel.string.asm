; Copyright 2023 Andreas Herzig
; Licence: MIT
%line 9003

; ks_itoa
;	input	eax	number
;		edi	pointer to buffer
;		edx	buffer size
;	output	edx	used buffer bytes
;	clobbers	edx
ks_itoa:
	;mov ebp, esp
	;mov eax, [ebp+4]
	;mov edi, [ebp+8]
	;mov edx, [ebp+12]
	
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
	je .end
	
	jmp .loop

.zero:
	dec ebp
	mov [ebp], byte '0'
	jmp .print_stack

.end:
	; calculate used bytes
	add esp, 16
	pop ebp
	pop edx
	pop ebx
	ret


ks_test_itoa:
	mov ebp, esp
	sub esp, 16
	push 16
	push ebp
	push 42
	call ks_itoa
	mov eax, ebp	; save buffer
	call puts
	mov esp, ebp	; restore stack
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