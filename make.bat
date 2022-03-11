@echo off

set NASM="C:\Tools\nasm\nasm.exe"

%NASM% -Wall -O0 -f bin src\kernel.asm -o out\kernel.bin

rem %NASM% --version