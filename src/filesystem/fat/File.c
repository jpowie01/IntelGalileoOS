/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include "File.h"
#include "../partitions/Gpt.h"

void printFATCatalog(CHAR16* directoryPath, EFI_HANDLE deviceHandle, EFI_BOOT_SERVICES* BootServices) {
	// Some logs
	Print(L"Listing all files from directory \"%s\"...\n", directoryPath);

	// Get interface for IO
	EFI_FILE_IO_INTERFACE* ioInterface = NULL;
	EFI_GUID simpleFileSystemProtocol = SIMPLE_FILE_SYSTEM_PROTOCOL;
	EFI_STATUS status = BootServices->HandleProtocol(deviceHandle, &simpleFileSystemProtocol, (void**)&ioInterface);
	if (status != EFI_SUCCESS) {
		Print(L"Handling protocol failed (Status: %x)\n", status);
		return;
	}

	// Get root directory
	EFI_FILE* root = NULL;
	status = ioInterface->OpenVolume(ioInterface, &root);
	if (status != EFI_SUCCESS) {
		Print(L"Getting root directory failed (Status: %x)\n", status);
		return;
	}

	// Get directory
	EFI_FILE* directory = NULL;
	status = root->Open(root, &directory, directoryPath, EFI_FILE_MODE_READ, 0);
	if (status != EFI_SUCCESS) {
		Print(L"Opening directory failed (Status: %x)\n", status);
		return;
	}

	// List all entries
	char directoryEntryBuffer[512];
	UINTN directoryBufferSize;
	EFI_FILE_INFO* directoryEntry = (EFI_FILE_INFO*)directoryEntryBuffer;
	while (1) {
		directoryBufferSize = sizeof(directoryEntryBuffer);
		status = directory->Read(directory, &directoryBufferSize, (void*)directoryEntry);
		if (status != EFI_SUCCESS) {
			Print(L"Error while reading directory entry (Status: %x)\n", status);
			break;
		}
		if (directoryBufferSize == 0) {
			break;
		}
		Print(L" - FileName: %s Size: %lx Attributes: %lx\n", directoryEntry->FileName, directoryEntry->FileSize, directoryEntry->Attribute);
	}
}
