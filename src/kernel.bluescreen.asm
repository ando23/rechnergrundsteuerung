; Copyright 2022-2023 Andreas Herzig
; Licence: MIT
;%line 3003


; Bluescreen anzeigen und CPUs anhalten
; input:	none
; output:	none
; clobbers:	none	
kernel_fail:
	; hier Bluescreen of death :-)
	mov word [0xB8000], 0x4020
	mov word [0xB8002], 0x4046
	mov word [0xB8004], 0x4061
	mov word [0xB8006], 0x4069
	mov word [0xB8008], 0x406c
	mov word [0xB800A], 0x4020
	jmp kernel_halt
