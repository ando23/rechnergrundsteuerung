; Copyright 2022 Andreas Herzig
; Licence: MIT

LINEAR_CODE_SELECTOR	equ 0x08
LINEAR_DATA_SELECTOR	equ 0x10

align 4, db 0
GDT_Descriptor:
	dw	8 * 3 - 1	; For limit storage
	dd	GDT_Table	; For base storage

align 4, db 0
GDT_Table:
	dw	0,0,0,0
	
	; 0x08 Code Ring 0 LINEAR_CODE_SELECTOR
	dw	0xffff	; Limit 0-15
	dw	0x0000	; Base 0-15
	db	0x00	; Base 16-23
	db	0b1001_1010	; Access Byte
	db	0b1100_1111	; Limit 16-19 und 4 bit Flags
	db	0x00	; Base 24-31

	; 0x10 Data Ring 0 LINEAR_DATA_SELECTOR
	dw	0xffff	; Limit 0-15
	dw	0x0000	; Base 0-15
	db	0x00	; Base 16-23
	db	0b1001_0010	; Access Byte
	db	0b1100_1111	; Limit 16-19 und 4 bit Flags
	db	0x00	; Base 24-31

	; Code Ring 3
	dw	0xffff	; Limit 0-15
	dw	0x0000	; Base 0-15
	db	0x00	; Base 16-23
	db	0b1111_1010	; Access Byte
	db	0b1100_1111	; Limit 16-19 und 4 bit Flags
	db	0x00	; Base 24-31

	; Data Ring 3
	dw	0xffff	; Limit 0-15
	dw	0x0000	; Base 0-15
	db	0x00	; Base 16-23
	db	0b1111_0010	; Access Byte
	db	0b1100_1111	; Limit 16-19 und 4 bit Flags
	db	0x00	; Base 24-31
	
	; Task State Segment
	dw	0,0,0,0

	; Other
	dw	0,0,0,0
	dw	0,0,0,0
	dw	0,0,0,0
	dw	0,0,0,0

	
struc gdte_t
.limitl:	resw 1
.basel:	resw 1
.basem:	resb 1
.access:	resb 1
.limithflags:	resb 1
.baseh:	resb 1
endstruc


;	clobbers	eax
init_GDT:
	lgdt [GDT_Descriptor]
	;ret

;	clobbers	eax
reload_segments:
	; Reload CS register containing code selector:
	JMP   LINEAR_CODE_SELECTOR:reload_CS ; 0x08 points at the new code selector
reload_CS:
	; Reload data segment registers:
	MOV   AX, LINEAR_DATA_SELECTOR ; 0x10 points at the new data selector
	MOV   DS, AX
	MOV   ES, AX
	MOV   FS, AX
	MOV   GS, AX
	MOV   SS, AX
	RET

