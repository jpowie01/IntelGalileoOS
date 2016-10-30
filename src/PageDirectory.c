/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include "PageDirectory.h"


PageEntry createTableEntry(void* address) {
	PageEntry pageEntry = (uint32_t)address & (0xFFFFF000);  // Removes last 12 bits from page table address for additional options
	pageEntry |= PRESENT | READ_WRITE | USER_SUPERVISOR;
	return pageEntry;
}

PageEntry* createPageTable(PageEntry* pageTableAddress, void* pageAddress) {
	for (int i = 0; i < PAGE_TABLE_LENGTH; i++) {
		pageTableAddress[i] = createTableEntry(pageAddress);
		pageAddress += PAGE_SIZE;
	}
	return pageTableAddress;
}

PageEntry* createPageDirectory(PageEntry* pageDirectoryAddress) {
	Print(L"Creating Page Directory...\n");
	PageEntry* pageTableAddress = pageDirectoryAddress + PAGE_TABLE_SIZE;
	void* pageAddress = 0x00000000;
	for (int i = 0; i < PAGE_TABLE_LENGTH; i++) {
		createPageTable(pageTableAddress, pageAddress);
		pageDirectoryAddress[i] = createTableEntry((void*)pageTableAddress);
		pageTableAddress += PAGE_TABLE_SIZE;
		pageAddress += PAGE_TABLE_LENGTH * PAGE_SIZE;
	}
	Print(L"Done.\n");
	return pageDirectoryAddress;
}
