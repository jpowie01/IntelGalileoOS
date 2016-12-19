/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include "File.h"

void printNTFSCatalog(CHAR16* directoryPath, EFI_HANDLE ImageHandle, EFI_BOOT_SERVICES* BootServices) {
	// Some logs
	Print(L"Listing all files from directory \"%s\"...\n", directoryPath);
}
