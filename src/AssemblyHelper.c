/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include "AssemblyHelper.h"


uint32_t getCR0() {
	uint32_t cr0;
	asm("mov %0, cr0;" : "=r" (cr0));
	return cr0;
}

uint32_t getCR3() {
	uint32_t cr3;
	asm("mov %0, cr3;" : "=r" (cr3));
	return cr3;
}

void setCR3(void* address) {
	asm(
		"mov eax, %0;"
		"mov cr3, eax;"
		:
		: "r" (address)
	);
}

void enablePaging() {
	asm(
		"mov eax, cr0;"
		"or eax, 0x80000000;"
		"mov cr0, eax;"
	);
}
