/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include "PageDirectory.h"


PageEntry createTableEntry(void* address, uint32_t flags) {
	PageEntry pageEntry = (uint32_t)address & (0xFFFFF000);  // Removes last 12 bits from page table address for additional options
	pageEntry |= flags;
	return pageEntry;
}

PageEntry* createPageTable(PageEntry* pageTableAddress, void* pageAddress) {
	for (int i = 0; i < PAGE_TABLE_LENGTH; i++) {
		pageTableAddress[i] = createTableEntry(pageAddress, EMPTY_TABLE_ENTRY_FLAGS);
		pageAddress += PAGE_SIZE;
	}
	return pageTableAddress;
}

PageEntry* createPageDirectory(PageEntry* pageDirectoryAddress) {
	PageEntry* pageTableAddress = pageDirectoryAddress + PAGE_TABLE_SIZE;
	void* pageAddress = 0x00000000;
	for (int i = 0; i < PAGE_TABLE_LENGTH; i++) {
		createPageTable(pageTableAddress, pageAddress);
		pageDirectoryAddress[i] = createTableEntry((void*)pageTableAddress, EMPTY_TABLE_ENTRY_FLAGS);
		pageTableAddress += PAGE_TABLE_SIZE;
		pageAddress += PAGE_TABLE_LENGTH * PAGE_SIZE;
	}
	return pageDirectoryAddress;
}

void* getPhysicalAddress(PageEntry* pageDirectoryAddress, void* linearAddress) {
	uint32_t pageDirectoryIndex = (uint32_t)linearAddress >> 22;  // Leave only first 10 bit from address
	uint32_t pageTableIndex = ((uint32_t)linearAddress >> 12) & 0x3FF;  // Leave only middle 10 bit from address
	uint32_t offset = (uint32_t)linearAddress & 0xFFF;  // Leave last 12 bits
	PageEntry* pageTable = (PageEntry*)(pageDirectoryAddress[pageDirectoryIndex] & 0xFFFFF000);  // Remove last 12 bits with options
	void* pageAddress = (void*)((uint32_t)pageTable[pageTableIndex] & 0xFFFFF000);  // Remove last 12 bits with options
	return pageAddress + offset;
}

void mapLinearAddress(PageEntry* pageDirectoryAddress, void* physicalAddress, void* linearAddress, uint32_t flags) {
	uint32_t pageDirectoryIndex = (uint32_t)linearAddress >> 22;  // Leave only first 10 bit from address
	uint32_t pageTableIndex = ((uint32_t)linearAddress >> 12) & 0x3FF;  // Leave only middle 10 bit from address
	PageEntry* pageTable = (PageEntry*)(pageDirectoryAddress[pageDirectoryIndex] & 0xFFFFF000);  // Remove last 12 bits with options
	pageTable[pageTableIndex] = createTableEntry(physicalAddress, EMPTY_TABLE_ENTRY_FLAGS);
}
