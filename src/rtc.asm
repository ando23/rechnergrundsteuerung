; Copyright 2022 Andreas Herzig
; Licence: MIT
%line 8003

; Info: https://wiki.osdev.org/...
; Info: PC Intern 4

; Benötigt: ...

RTC_CMD	equ 0x70
RTC_DATA	equ 0x71

RTC_FUNC_CUR_SEC	equ 0x00
RTC_FUNC_ALARM_SEC	equ 0x01
RTC_FUNC_CUR_MIN	equ 0x02
RTC_FUNC_ALARM_MIN	equ 0x03
RTC_FUNC_CUR_HOUR	equ 0x04
RTC_FUNC_ALARM_HOUR	equ 0x05
RTC_FUNC_CUR_DOW	equ 0x06
RTC_FUNC_CUR_DAY	equ 0x07
RTC_FUNC_CUR_MONTH	equ 0x08
RTC_FUNC_CUR_YEAR	equ 0x09
RTC_FUNC_STATEREG_A	equ 0x0A
RTC_FUNC_STATEREG_B	equ 0x0B
RTC_FUNC_STATEREG_C	equ 0x0C
RTC_FUNC_STATEREG_D	equ 0x0D
RTC_FUNC_CUR_EPOQUE	equ 0x32

rtc_year:	db 0x00
rtc_mon:	db 0x00
rtc_day:	db 0x00
rtc_hr:	db 0x00
rtc_min:	db 0x00
rtc_sec:	db 0x00
rtc_dow:	db 0x00

init_rtc:
	cli
	push eax

	mov al, 0x8A
	out RTC_CMD, al
	mov al, 0x20
	out RTC_DATA, al
	
	; disable BCD, 24h
	mov al, RTC_FUNC_STATEREG_B
	out RTC_CMD, al
	in al, RTC_DATA
	or al, 0b0000_0110
	out RTC_DATA, al

	pop eax
	sti
	ret

	
rtc_update:
	push eax
	
	mov al, RTC_FUNC_CUR_YEAR
	out RTC_CMD, al
	in al, RTC_DATA
	mov [rtc_year], al

	mov al, RTC_FUNC_CUR_MONTH
	out RTC_CMD, al
	in al, RTC_DATA
	mov [rtc_mon], al

	mov al, RTC_FUNC_CUR_DAY
	out RTC_CMD, al
	in al, RTC_DATA
	mov [rtc_day], al

	mov al, RTC_FUNC_CUR_HOUR
	out RTC_CMD, al
	in al, RTC_DATA
	mov [rtc_hr], al

	mov al, RTC_FUNC_CUR_MIN
	out RTC_CMD, al
	in al, RTC_DATA
	mov [rtc_min], al

	mov al, RTC_FUNC_CUR_SEC
	out RTC_CMD, al
	in al, RTC_DATA
	mov [rtc_sec], al

	pop eax
	ret

rtc_print:
	push eax
	
	mov eax, msg_rtc
	call kputs

	mov eax, 0
		mov al, 20
		call kputb_dec
	mov al, [rtc_year]
	;add eax, 2000
	call kputb_dec
	mov al, '-'
	call kputc
	mov al, [rtc_mon]
	call kputb_dec
	mov al, '-'
	call kputc
	mov al, [rtc_day]
	call kputb_dec
	mov al, ' '
	call kputc
	mov al, [rtc_hr]
	call kputb_dec
	mov al, ':'
	call kputc
	mov al, [rtc_min]
	call kputb_dec
	mov al, ':'
	call kputc
	mov al, [rtc_sec]
	call kputb_dec
	
	mov eax, msg_rtc2
	call kputs
	
	pop eax
	ret


msg_rtc:	db "Aktuelle Zeit: ",0
msg_rtc2:	db 13,10, 0
