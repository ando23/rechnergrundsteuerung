#pragma once
#ifndef ___MAKROS_H___
#define ___MAKROS_H___

#define SYSA_KERNEL_NORETURN	__attribute__ ((noreturn))
#define SYSA_KERNEL_INTERRUPT	__attribute__ ((interrupt))

#define STRUCT_PACKED	__attribute__ ((packed))

#define LINEAR_CODE_SELECTOR	0x08
#define LINEAR_DATA_SELECTOR	0x10


#define IS_BIT_SET(flags, bit) (flags & (1<<bit))


#endif
