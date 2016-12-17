/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include <Uefi.h>
#include <Library/UefiLib.h>

#include "PageDirectory.h"
#include "AssemblyHelper.h"
#include "File.h"


EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	// Initialize
	Print(L"Starting...\n");

	// Prepare Page Directory with all Page Tables
	Print(L"CR0: %x CR3: %x\n", getCR0(), getCR3());
	createPageDirectory(PAGE_DIRECTORY_ADDRESS);
	setCR3((void*)PAGE_DIRECTORY_ADDRESS);
	Print(L"Everything is prepared.\n");

	// Turn on paging
	Print(L"Enabling paging...\n");
	enablePaging();
	Print(L"CR0: %x CR3: %x\n", getCR0(), getCR3());
	Print(L"Done!\nShould work like a charm!\n");

	// Test everything
	Print(L"Testing...\n");
	Print(L"Physical address for linear address (0xE0000000): %x\n", getPhysicalAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xE0000000));
	Print(L"Physical address for linear address (0xD0000000): %x\n", getPhysicalAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xD0000000));
	Print(L"Mapping linear address (0xE0000000) to physical address (0xD0000000)...\n");
	mapLinearAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xD0000000, (void*)0xE0000000, EMPTY_TABLE_ENTRY_FLAGS);
	Print(L"Mapping linear address (0xD0000000) to physical address (0xE0000000)...\n");
	mapLinearAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xE0000000, (void*)0xD0000000, EMPTY_TABLE_ENTRY_FLAGS);
	Print(L"Physical address for linear address (0xE0000000): %x\n", getPhysicalAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xE0000000));
	Print(L"Physical address for linear address (0xD0000000): %x\n", getPhysicalAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xD0000000));
	Print(L"Done.\n");

	// Print list of all files in a root directory
	printCatalog(L"\\", ImageHandle, SystemTable->BootServices);

	// End of program
	Print(L"Exit...\n");
	return EFI_SUCCESS;
}
