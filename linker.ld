ENTRY(_start)

SECTIONS
{
	. = 2M;
	
	tag_start = .;
	
	.text BLOCK(4K) : ALIGN(4K)
	{
		. = ALIGN(8);
		KEEP(*(.multiboot))
		*(.text)
	}
	
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
	
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
	
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
	
	tag_end = .;
}