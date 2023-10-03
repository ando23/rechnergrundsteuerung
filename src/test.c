

void foo() {
	// Pointer auf VMEM
	char *p = (char *)0xb8000;
	
	for (int i=0; i<26; i++) {
		*p++ = 0x4f;
		*p++ = 65 + i;
	}
}
