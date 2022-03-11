; Copyright 2022 Andreas Herzig
; Licence: MIT

%macro OUTB 2
	mov dx, %1
	mov al, %2
	out dx, al
%endmacro

%macro INB 2
	mov dx, %1
	in al, dx
%endmacro


check_cpu:
	; Hersteller
	mov eax, 0
	cpuid			; eax hat hier höchstmöglichen Wert für CPUID
	
	mov [cpuinfo + 0], ebx
	mov [cpuinfo + 4], edx
	mov [cpuinfo + 8], ecx
	mov eax, cpuinfo1
	call kputs

	; Features
	mov eax, 1
	cpuid
	push eax
	mov eax, cpuinfo2
	call kputs
	pop eax
	call kputl
	mov eax, cpuinfo3
	call kputs
	mov eax, edx
	call kputl
	mov eax, cpuinfo4
	call kputs
	mov eax, ecx
	call kputl
	mov eax, cpuinfonl
	call kputs
	
	ret


cpuinfo1:	db "CPU-Info: "
cpuinfo:	times 12 db 0
	db 13,10,0
cpuinfo2:	db "CPU-Info: Flag EAX=", 0
cpuinfo3:	db " EDX=", 0
cpuinfo4:	db " ECX=", 0
cpuinfonl:	db 13,10,0

enter_pmode:
	call	enable_A20
	;call	init_GDT
	
	; enable protected mode
	mov	eax, cr0
	or	al, 1
	mov	cr0, eax

	jmp	08h:kernel_pmode	; set CS
kernel_pmode:
	;call	reload_segments	; set other segments

	ret


enable_A20:
	in	al, 0x92
	or	al, 2
	out	0x92, al
	ret

init_sse:
	mov	eax, cr0
	and	ax, 0b1111_1101
	or	ax, 0b0000_0001
	mov	cr0, eax
	
	mov	eax, cr4
	or	ax, 0b11_0000_0000
	mov	cr4, eax
	
	ret
