/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#ifndef _ASSEMBLY_HELPER_H
#define _ASSEMBLY_HELPER_H

#include <stdint.h>

extern uint32_t getCR0();
extern uint32_t getCR3();
extern void setCR3(void* address);
extern void enablePaging();

#endif
