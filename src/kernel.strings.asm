; Copyright 2022 Andreas Herzig
; Licence: MIT

section .data
%define NL 10
msg_hello:	db NL, " [SYS/A v0.1] ", NL, NL, 0
%define NL 13,10
msg_ser:	db "[",0x1b,"[33mSYS/A",0x1b,"[36m] Starte ...", NL, 0
msg_cpu:	db "CPU ...", NL, 0
msg_pmode:	db "Wechsel in den Protected Mode ...", NL, 0
msg_ints:	db "Richte Int-Handler ein ...", NL, 0
msg_pit:	db "Initialisiere Timer ...", NL, 0
msg_init_rtc:	db "Uhr einstellen...", NL, 0
msg_init_done:	db "Initialisierung vollstaendig. Viel Spass!", NL, NL, 0
msg_end:	db "Fahre herunter.", NL, 0
msg_dbg1:	db "  ",0x1b,"[31;1mDEBUG #1", NL, 0
msg_dbg2:	db "  ",0x1b,"[31;1mDEBUG #2", NL, 0
msg_dbg3:	db "  ",0x1b,"[31;1mDEBUG #3", NL, 0
msg_dbg4:	db "  ",0x1b,"[31;1mDEBUG #4", NL, 0
msg_dbg5:	db "  ",0x1b,"[31;1mDEBUG #5", NL, 0
