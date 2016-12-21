/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include "File.h"
#include "../partitions/Gpt.h"

void printNTFSCatalog(CHAR16* directoryPath, EFI_HANDLE deviceHandle, EFI_BOOT_SERVICES* BootServices) {
	// Some logs
	Print(L"Listing all files from directory \"%s\"...\n", directoryPath);

	// Get interface for IO
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BLOCK_IO_PROTOCOL* ioInterface = NULL;
	EFI_GUID blockIoProtocol = EFI_BLOCK_IO_PROTOCOL_GUID;
	status = BootServices->HandleProtocol(deviceHandle, &blockIoProtocol, (void**)&ioInterface);
	if (status != EFI_SUCCESS) {
		Print(L"Handling protocol failed (Status: %x)\n", status);
		return;
	}

	// Read bytes from disk
	EFI_BLOCK_IO_MEDIA* media = ioInterface->Media;
	UINTN bufferSize = 512;
	UINT8 buffer[bufferSize];
	status = ioInterface->ReadBlocks(ioInterface, media->MediaId, 0, bufferSize, buffer);
	if (status != EFI_SUCCESS) {
		Print(L"Reading from disk failed (Status: %x)\n", status);
		return;
	}
	for (int row = 0; row < bufferSize / 16; row++) {
		for (int col = 0; col < 16; col++) {
			Print(L"%02x ", buffer[row*16 + col]);
		}
		Print(L"\n");
	}
}
