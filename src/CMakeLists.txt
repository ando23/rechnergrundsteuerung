set(SOURCES
	kernel.c
	memory.c
	serial.c
	keymappings.c
	textmode.c
	cpu.c
	gfx.c
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nolibc")
sysa_lib(LibC c)
target_link_libraries(LibC PRIVATE)
