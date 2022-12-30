@echo off

set QEMU="C:\Tools\qemu\qemu-system-i386.exe"
set KRNL="out\kernel.bin"

rem https://wiki.osdev.org/Qemu
set DBGTRIFAULT=-no-shutdown -no-reboot -d int

%QEMU% -kernel %KRNL% -m 16m  -serial stdio -rtc base=localtime

