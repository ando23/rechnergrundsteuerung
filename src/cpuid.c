#include "cpuid.h"
#include "string.h"
#include "serial.h"

static inline
void get_cpuid(uint32_t code, uint32_t *a, uint32_t *d, uint32_t *c)
{
	asm volatile("cpuid" : "=a"(*a),"=d"(*d),"=c"(*c) : "a"(code) : "ebx");
}

static inline
void get_cpuid_string(uint32_t code, uint32_t where[4])
{
	asm volatile("cpuid" : "=a"(*where),"=b"(*(where+1)),"=d"(*(where+2)),"=c"(*(where+3)) : "a"(code));
}
bool cpu_has_feature_edx(struct cpu_info_t *ci, uint32_t feature) {
	if (!ci) return false;
	return ci->feature_flags_edx & feature;
}
bool cpu_has_feature_ecx(struct cpu_info_t *ci, uint32_t feature) {
	if (!ci) return false;
	return ci->feature_flags_ecx & feature;
}

void cpu_cpuid(uint32_t func, uint8_t* data) {
	uint32_t a = func;
	uint32_t b = 0;
	uint32_t c = 0;
	uint32_t d = 0;
	asm (
		"mov eax, %0\n\t"
		"cpuid\n\t"
		"mov %0, eax\n\t"
		"mov %1, ebx\n\t"
		"mov %2, ecx\n\t"
		"mov %3, edx\n\t"
		: "=r" (a), "=r" (b), "=r" (c), "=r" (d)
		: "0" (a)
	);
	uint32_t* ptr = (uint32_t*)data;
	ptr[0] = a;
	ptr[1] = b;
	ptr[2] = c;
	ptr[3] = d;
}

void check_cpu(cpu_info_t* data) {
	if (!data) return;
	
	memset(data, 0, sizeof(data));
	
	for (size_t i=0; i<sizeof(data->vendor); i++)
		data->vendor[i] = 0;

	get_cpuid_string(CPUID_GETVENDORSTRING, data->func0result);
	
	if (data->max_function_id >= CPUID_GETFEATURES)
		get_cpuid(CPUID_GETFEATURES, &data->processor_info, &data->feature_flags_edx, &data->feature_flags_ecx);
	
	//cpu_cpuid(CPUID_GETVENDORSTRING,	data->manufacturer);
	//cpu_cpuid(CPUID_GETFEATURES,	data->features.ptr);
}

void check_features(cpu_info_t* ci) {
	check_cpu(ci);
	
	kputs( "Manufacturer: " );
	for (size_t i=0; i < 13; i++) {
		kputc( ci->vendor[i] );
	}
	kputnl();
	
	kputs( "Features EDX: ");
	{
		if (ci->fpu)	kputs2("fpu ");
		if (ci->vme)	kputs2("vme ");
		if (ci->de)	kputs2("de  ");
		if (ci->pse)	kputs2("pse ");
			
		if (ci->tsc)	kputs2("tsc ");
		if (ci->msr)	kputs2("msr ");
		if (ci->pae)	kputs2("pae ");
		if (ci->mce)	kputs2("mce ");
			
		if (ci->cx8)	kputs2("cx8 ");
		if (ci->apic)	kputs2("apc ");
		if (ci->sep)	kputs2("sep ");
			
		if (ci->mtrr)	kputs2("mtr ");
		if (ci->pge)	kputs2("pge ");
		if (ci->mca)	kputs2("mca ");
		if (ci->cmov)	kputs2("cmv ");
			
		if (ci->pat)	kputs2("pat ");
		if (ci->pse36)	kputs2("pse ");
		if (ci->psn)	kputs2("psn ");
		if (ci->clfsh)	kputs2("cfl ");
			
		if (ci->ds)	kputs2("ds ");
		if (ci->acpi)	kputs2("acp ");
		if (ci->mmx)	kputs2("mmx ");
			
		if (ci->fxsr)	kputs2("fxs ");
		if (ci->sse)	kputs2("sse ");
		if (ci->sse2)	kputs2("ss2 ");
		if (ci->ss)	kputs2("ss  ");
			
		if (ci->htt)	kputs2("htt ");
		if (ci->tm)	kputs2("tm  ");
		if (ci->ia64)	kputs2("i64 ");
		if (ci->pbe)	kputs2("pbe ");
	}
	kputnl();

	kputs( "Features ECX: ");
	{
		if (ci->sse3)	kputs2("ss3 ");
		if (ci->pclmulqdq)	kputs2("mul ");
		if (ci->dtes64)	kputs2("d64 ");
		if (ci->monitor)	kputs2("mon ");
			
		if (ci->ds_cpl)	kputs2("dsc ");
		if (ci->vmx)	kputs2("vmx ");
		if (ci->smx)	kputs2("smx ");
		if (ci->est)	kputs2("est ");
			
		if (ci->tm2)	kputs2("tm2 ");
		if (ci->ssse3)	kputs2("s3s ");
		if (ci->cnxt_id)	kputs2("l1c ");
		if (ci->sdbg)	kputs2("sdg ");
			
		if (ci->fma)	kputs2("fma ");
		if (ci->cx16)	kputs2("c16 ");
		if (ci->xtpr)	kputs2("xtp ");
		if (ci->pdcm)	kputs2("pdc ");
			
		if (ci->pcid)	kputs2("pci ");
		if (ci->psn)	kputs2("dca ");
		if (ci->sse41)	kputs2("s41 ");
			
		if (ci->sse42)	kputs2("s42 ");
		if (ci->x2apic)	kputs2("x2a ");
		if (ci->movbe)	kputs2("mov ");
		if (ci->popcnt)	kputs2("pop ");
			
		if (ci->tsc_deadline)	kputs2("tsc ");
		if (ci->aes)	kputs2("aes ");
		if (ci->xsave)	kputs2("xsv ");
		if (ci->osxsave)	kputs2("osx ");
			
		if (ci->avx)	kputs2("avx ");
		if (ci->f16c)	kputs2("f16 ");
		if (ci->rdrnd)	kputs2("rnd ");
		if (ci->hypervisor)	kputs2("hyp ");
	}
	kputnl();
}

