@echo off

rem configuration
	set NASM="C:\Tools\nasm\nasm.exe"
	set OUT=out

rem create output dir if not exist
	if not exist %OUT% mkdir %OUT%

rem build it
	rem %NASM% --version
	%NASM% -Wall -O0 -f bin src\kernel.asm -o %OUT%\kernel.bin
