// Copyright 2022 Andreas Herzig
// Licence: MIT

//	`23456789ABCD
//	GHIJKLMNOPQR
//	UVWXYZ[\]^_b
//	cdefghijkl
//
//	ESC = 1
//	ENTER = S
//	SPACE = p
//	BS = E
//	CTRL = T
//	ALT = o
//	TAB = F
//	CAPSLOCK = q
//	SHIFT_LEFT = a
//	SHIFT_RIGHT = m
//
//	F1 - F4 = r s t u
//	F5 - F8 = v w x y
//	F9 - F10 = z {


// Tastenmapping für deutsch
const uint8_t KEYTABLE[] = {
	0, 0, 0, 0, 0, 0, 0, 0	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 0	0x00
	0, 0, 0, 0, 0, 0, 0, 0	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 16	0x10
	0, 0, '1', '2', '3', '4', '5', '6',	'7', '8', 0, 0, 0, 0, 0, 0	// OFFSET 32	0x20
	0, 0, 0, 0, 0, 0, 0, 0	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 48	0x30
			
	0, '9', '0', 0, 0, 0x08, 0, 'Q',	'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O'	// OFFSET 64	0x40
	'P', 0, 0, 0x10, 0, 'A', 'S', 'D',	'F', 'G', 'H', 'J', 'K', 'L', 0, 0	// OFFSET 80	0x50
	0, 0, 0, 'Y', 'X', 'C', 'V', 'B',	'N', 'M', 0, 0, 0, 0, 0, 0	// OFFSET 96	0x60
	0x20, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 112	0x70
			
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 128	0x80
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 144	0x90
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 160	0xA0
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 176	0xB0
			
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 192	0xC0
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 208	0xD0
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 224	0xE0
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, 0, 0, 0	// OFFSET 240	0xF0
};



