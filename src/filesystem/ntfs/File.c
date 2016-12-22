/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include "File.h"
#include "../partitions/Gpt.h"

UINT64 readLittleEndianFromBuffer(UINT8* buffer, UINT8 startingPosition, UINT8 bytes) {
	UINT64 result = buffer[startingPosition];
	for (UINT8 byte = 1; byte < bytes; byte++) {
		result += ((UINT64)buffer[startingPosition + byte] << 8 * byte);
	}
	return result;
}

INT8 absoluteValue(INT8 value) {
	if (value < 0) {
		return -value;
	}
	return value;
}

INT64 powerOfTwo(INT8 power) {
	INT64 result = 1;
	while (power-- > 0) result *= 2;
	return result;
}

void printNTFSCatalog(CHAR16* directoryPath, EFI_HANDLE deviceHandle, EFI_BOOT_SERVICES* BootServices) {
	// Some logs
	Print(L"Listing all files from directory \"%s\"...\n\n", directoryPath);

	// Get interface for IO
	EFI_STATUS status = EFI_SUCCESS;
	EFI_BLOCK_IO_PROTOCOL* ioInterface = NULL;
	EFI_GUID blockIoProtocol = EFI_BLOCK_IO_PROTOCOL_GUID;
	status = BootServices->HandleProtocol(deviceHandle, &blockIoProtocol, (void**)&ioInterface);
	if (status != EFI_SUCCESS) {
		Print(L"Handling protocol failed (Status: 0x%08x)\n", status);
		return;
	}

	// Read bytes from disk
	EFI_BLOCK_IO_MEDIA* media = ioInterface->Media;
	UINTN bufferSize = 512;
	UINT8 buffer[bufferSize];
	status = ioInterface->ReadBlocks(ioInterface, media->MediaId, 0, bufferSize, buffer);
	if (status != EFI_SUCCESS) {
		Print(L"Reading from disk failed (Status: 0x%08x)\n", status);
		return;
	}

	// Read basic data
	UINT16 bytesPerSector = readLittleEndianFromBuffer(buffer, 0x0B, 2);  // 0x200
	UINT8 sectorsPerCluster = readLittleEndianFromBuffer(buffer, 0x0D, 1);  // 0x8
	UINT64 MFTClusterNumber = readLittleEndianFromBuffer(buffer, 0x30, 8);  // 0x4
	INT8 clustersPerMFTRecordRaw = readLittleEndianFromBuffer(buffer, 0x40, 1);  // 0xF6 -> 0x400 (1024 bytes)
	UINT64 sectorsPerMFTRecord = clustersPerMFTRecordRaw * sectorsPerCluster;  // stored in bytes
	if (clustersPerMFTRecordRaw < 0) {
		sectorsPerMFTRecord = powerOfTwo(absoluteValue(clustersPerMFTRecordRaw)) / 512;  // FIXME: 512 = bytesPerSector
	}
	Print(L"Sector: %dB\n", bytesPerSector);
	Print(L"Sectors per cluster: %d\n", sectorsPerCluster);
	Print(L"MFT cluster number: %d\n", MFTClusterNumber);
	Print(L"Sectors per MFT record: %d\n", sectorsPerMFTRecord);

	// Read block of root folder
	UINT64 rootFolderSector = MFTClusterNumber * sectorsPerCluster + 5 * sectorsPerMFTRecord;
	status = ioInterface->ReadBlocks(ioInterface, media->MediaId, rootFolderSector, bufferSize, buffer);
	if (status != EFI_SUCCESS) {
		Print(L"Reading from disk failed (Status: %x)\n", status);
		return;
	}

	// Print first 4 rows of data from root directory
	Print(L"\nFirst %d bytes from root directory\n", 4*16);
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 16; col++) {
			Print(L"%02x ", buffer[row*16 + col]);
		}
		Print(L"\n");
	}
}
