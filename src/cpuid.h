#include <stdbool.h>
#include <stdint.h>
#include "makros.h"


enum cpuid_requests {
	CPUID_GETVENDORSTRING = 0,
	CPUID_GETFEATURES = 1,
	CPUID_GETTLB = 2,
	CPUID_GETSERIAL = 3,

	CPUID_INTELEXTENDED = 0x80000000,
	CPUID_INTELFEATURES,
	CPUID_INTELBRANDINGSTRING,
	CPUID_INTELBRANDSTRINGMORE,
	CPUID_INTELBRANDSTRINGEND,
};

enum cpuid_feature_flags {
    CPUID_FEAT_ECX_SSE3         = 1 << 0, 
    CPUID_FEAT_ECX_PCLMUL       = 1 << 1,
    CPUID_FEAT_ECX_DTES64       = 1 << 2,
    CPUID_FEAT_ECX_MONITOR      = 1 << 3,  
    CPUID_FEAT_ECX_DS_CPL       = 1 << 4,  
    CPUID_FEAT_ECX_VMX          = 1 << 5,  
    CPUID_FEAT_ECX_SMX          = 1 << 6,  
    CPUID_FEAT_ECX_EST          = 1 << 7,  
    CPUID_FEAT_ECX_TM2          = 1 << 8,  
    CPUID_FEAT_ECX_SSSE3        = 1 << 9,  
    CPUID_FEAT_ECX_CID          = 1 << 10,
    CPUID_FEAT_ECX_FMA          = 1 << 12,
    CPUID_FEAT_ECX_CX16         = 1 << 13, 
    CPUID_FEAT_ECX_ETPRD        = 1 << 14, 
    CPUID_FEAT_ECX_PDCM         = 1 << 15, 
    CPUID_FEAT_ECX_PCIDE        = 1 << 17, 
    CPUID_FEAT_ECX_DCA          = 1 << 18, 
    CPUID_FEAT_ECX_SSE4_1       = 1 << 19, 
    CPUID_FEAT_ECX_SSE4_2       = 1 << 20, 
    CPUID_FEAT_ECX_x2APIC       = 1 << 21, 
    CPUID_FEAT_ECX_MOVBE        = 1 << 22, 
    CPUID_FEAT_ECX_POPCNT       = 1 << 23, 
    CPUID_FEAT_ECX_AES          = 1 << 25, 
    CPUID_FEAT_ECX_XSAVE        = 1 << 26, 
    CPUID_FEAT_ECX_OSXSAVE      = 1 << 27, 
    CPUID_FEAT_ECX_AVX          = 1 << 28,
 
    CPUID_FEAT_EDX_FPU          = 1 << 0,  
    CPUID_FEAT_EDX_VME          = 1 << 1,  
    CPUID_FEAT_EDX_DE           = 1 << 2,  
    CPUID_FEAT_EDX_PSE          = 1 << 3,  
    CPUID_FEAT_EDX_TSC          = 1 << 4,  
    CPUID_FEAT_EDX_MSR          = 1 << 5,  
    CPUID_FEAT_EDX_PAE          = 1 << 6,  
    CPUID_FEAT_EDX_MCE          = 1 << 7,  
    CPUID_FEAT_EDX_CX8          = 1 << 8,  
    CPUID_FEAT_EDX_APIC         = 1 << 9,  
    CPUID_FEAT_EDX_SEP          = 1 << 11, 
    CPUID_FEAT_EDX_MTRR         = 1 << 12, 
    CPUID_FEAT_EDX_PGE          = 1 << 13, 
    CPUID_FEAT_EDX_MCA          = 1 << 14, 
    CPUID_FEAT_EDX_CMOV         = 1 << 15, 
    CPUID_FEAT_EDX_PAT          = 1 << 16, 
    CPUID_FEAT_EDX_PSE36        = 1 << 17, 
    CPUID_FEAT_EDX_PSN          = 1 << 18, 
    CPUID_FEAT_EDX_CLF          = 1 << 19, 
    CPUID_FEAT_EDX_DTES         = 1 << 21, 
    CPUID_FEAT_EDX_ACPI         = 1 << 22, 
    CPUID_FEAT_EDX_MMX          = 1 << 23, 
    CPUID_FEAT_EDX_FXSR         = 1 << 24, 
    CPUID_FEAT_EDX_SSE          = 1 << 25, 
    CPUID_FEAT_EDX_SSE2         = 1 << 26, 
    CPUID_FEAT_EDX_SS           = 1 << 27, 
    CPUID_FEAT_EDX_HTT          = 1 << 28, 
    CPUID_FEAT_EDX_TM1          = 1 << 29, 
    CPUID_FEAT_EDX_IA64         = 1 << 30,
    CPUID_FEAT_EDX_PBE          = 1 << 31
};

typedef struct cpu_info_t
{
	// 00
	union {
		uint32_t func0result[4];
		struct {
			uint32_t max_function_id;
			char vendor[13];
		};
	};
	
	// 01
	union {
		uint32_t processor_info;	// eax
	};
	union {
		uint32_t feature_flags_edx;
		struct {
			int fpu : 1;
			int vme : 1;
			int de : 1;
			int pse : 1;
			int tsc : 1;
			int msr : 1;
			int pae : 1;
			int mce : 1;
			int cx8 : 1;
			int apic : 1;
			int res1 : 1;
			int sep : 1;		// Extended: syscall
			int mtrr : 1;
			int pge : 1;
			int mca : 1;
			int cmov : 1;
			int pat : 1;
			int pse36 : 1;
			int psn : 1;		// Extended: reserved
			int clfsh : 1;		// Extended: mp
			int res2 : 1;		// Extended: nx
			int ds : 1;			// Extended: reserved
			int acpi : 1;		// Extended: mmxext
			int mmx : 1;
			int fxsr : 1;
			int sse : 1; 		// Extended: fxsr_opt
			int sse2 : 1; 		// Extended: pdpe1gb
			int ss : 1; 		// Extended: rdtscp
			int htt : 1; 		// Extended: reserved
			int tm : 1; 		// Extended: lm
			int ia64 : 1; 		// Extended: 3dnowext
			int pbe : 1; 		// Extended: 3dnow
		};
	};
	union {
		uint32_t feature_flags_ecx;
		struct {
			int sse3 : 1; 		// Extended: lahf_lm
			int pclmulqdq : 1; 	// Extended: cmp_legacy
			int dtes64 : 1;		// Extended: svm
			int monitor : 1;	// Extended: extapic
			int ds_cpl : 1;		// Extended: cr8_legacy
			int vmx : 1; 		// Extended: abm
			int smx : 1; 		// Extended: sse4a
			int est : 1; 		// Extended: misalignsse
			int tm2 : 1; 		// Extended: 3dnowprefetch
			int ssse3 : 1; 		// Extended: osvw
			int cnxt_id : 1; 		// Extended: ibs
			int sdbg : 1; 		// Extended: xop
			int fma : 1; 		// Extended: skinit
			int cx16 : 1; 		// Extended: wdt
			int xtpr : 1; 		// Extended: reserved
			int pdcm : 1; 		// Extended: lwp
			int res3 : 1; 		// Extended: fma4
			int pcid : 1; 		// Extended: tce
			int dca : 1; 		// Extended: reserved
			int sse41 : 1; 		// Extended: nodeid_msr
			int sse42 : 1; 		// Extended: reserved
			int x2apic : 1; 		// Extended: tbm
			int movbe : 1; 		// Extended: topoext
			int popcnt : 1; 		// Extended: perfctr_core
			int tsc_deadline : 1; 		// Extended: perfctr_nb
			int aes : 1; 		// Extended: reserved
			int xsave : 1; 		// Extended: dbx
			int osxsave : 1; 		// Extended: perftsc
			int avx : 1; 		// Extended: pcx_l2i
			int f16c : 1; 		// Extended: reserved
			int rdrnd : 1; 		// Extended: reserved
			int hypervisor : 1; 		// Extended: reserved
		};
	};
	uint32_t apic_id;	// ebx
	
	// 02	Cache und TLB descriptor
	// 03	Processor serial number
	// 04 0B	Intel Thread/Core topology
	// 06	Thermal und Power Management
	// 07 00	Extended Features
	// 07 01	Extended Features
} /*STRUCT_PACKED*/ cpu_info_t;

/*
typedef struct {
	uint8_t manufacturer[4*32];
	union {
		uint8_t ptr[4*32];
		struct {
			uint32_t a,b,c,d;
		};
	} features;
} cpu_info_t;
*/

void check_cpu(cpu_info_t* data);
void check_features(cpu_info_t* data);

