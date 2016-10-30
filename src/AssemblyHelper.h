/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#ifndef _ASSEMBLY_HELPER_H
#define _ASSEMBLY_HELPER_H

#include <stdint.h>

uint32_t getCR0();
uint32_t getCR3();
void setCR3(void* address);
void enablePaging();

#endif
