/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#ifndef _PAGE_DIRECTORY_H
#define _PAGE_DIRECTORY_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <stdint.h>
#include <stdlib.h>


typedef uint32_t PageEntry;

#define PRESENT                 1 << 0
#define READ_WRITE              1 << 1  // 0 = READ, 1 = READ & WRITE
#define USER_SUPERVISOR         1 << 2  // 0 = USER, 1 = SUPERVISOR
#define PAGE_WRITE_TRANSPARENT  1 << 3
#define PAGE_CACHE_DISABLE      1 << 4
#define ACCESSED                1 << 5
#define DIRTY                   1 << 6

#define OS_AVAILABLE_START      9
#define OS_AVAILABLE_LENGTH     3
#define ADDRESS_START           12

#define PAGE_DIRECTORY_ADDRESS  (PageEntry*)0x03000000
#define PAGE_ENTRY_SIZE         sizeof(PageEntry)
#define PAGE_TABLE_LENGTH       1024
#define PAGE_TABLE_SIZE         PAGE_TABLE_LENGTH * PAGE_ENTRY_SIZE
#define PAGE_SIZE               4096  // 4 KB

#define EMPTY_TABLE_ENTRY_FLAGS PRESENT | READ_WRITE | USER_SUPERVISOR


PageEntry createTableEntry(void* address, uint32_t flags);
PageEntry* createPageTable(PageEntry* pageTableAddress, void* pageAddress);
PageEntry* createPageDirectory(PageEntry* pageDirectoryAddress);

void* getPhysicalAddress(PageEntry* pageDirectoryAddress, void* linearAddress);
void mapLinearAddress(PageEntry* pageDirectoryAddress, void* physicalAddress, void* linearAddress, uint32_t flags);

#endif
