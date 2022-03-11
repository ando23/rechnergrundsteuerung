; Copyright 2022 Andreas Herzig
; Licence: MIT

; Info: https://wiki.osdev.org/PIT

PIT_CHN0	equ 0x40
PIT_CHN1	equ 0x41
PIT_CHN2	equ 0x42
PIT_CMD	equ 0x43

PIT_OS_FLAGS equ	0b00010110
;	  xx......	channel
;	  ..xx....	access mode, 01=lobyte only
;	  ....xxx.	operating mode, 3=square wave
;	  .......0	0=binary 1=BCD

; Initialise the Programmable Interrupt Timer
init_pit:
	pushad
	
	
	
	pushfd
	cli
	
	OUTB PIT_CMD, PIT_OS_FLAGS
	
	sti
	popfd
	
	popad
	ret
