/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include "Gpt.h"

UINTN amountOfPartitions = 0;
GPTPartition listOfPartitions[MAX_AMOUNT_OF_PARTITIONS];


BOOLEAN isBlockIOProtocol(EFI_GUID *guid) {
  EFI_GUID blockIoProtocol = EFI_BLOCK_IO_PROTOCOL_GUID;
  EFI_GUID* blockIoProtocolPtr = &blockIoProtocol;
  for (UINTN i = 0; i < 16; i++) {
    if (*(((UINT8*) guid) + i) != *(((UINT8*) blockIoProtocolPtr) + i)) {
      return FALSE;
    }
  }
  return TRUE;
}

BOOLEAN isNTFS(UINT8* header) {
  UINT8 NTFSHeader[] = {0xEB, 0x52, 0x90, 0x4E, 0x54, 0x46, 0x53, 0x20, 0x20, 0x20, 0x20};
  for (UINTN i = 0; i < 10; i++) {
    if (header[i] != NTFSHeader[i]) {
      return FALSE;
    }
  }
  return TRUE;
}

BOOLEAN isFAT(UINT8* header) {
  if (header[0] != 0xEB) return FALSE;
  if (header[1] != 0x3C) return FALSE;
  if (header[2] != 0x90) return FALSE;
  if (header[510] != 0x55) return FALSE;
  if (header[511] != 0xAA) return FALSE;
  return TRUE;
}

void prepareListOfGptPartitions(EFI_BOOT_SERVICES* BootServices) {
  // Clear the amount of partitions
  amountOfPartitions = 0;

  // Read all handles to devices
  UINTN amountOfHandles;
  EFI_HANDLE *handleBuffer;
  EFI_STATUS status = BootServices->LocateHandleBuffer(AllHandles, NULL, NULL, &amountOfHandles, &handleBuffer);
  if (status != EFI_SUCCESS) {
    Print(L"Could not read handles to devices (status: %x)!\n", status);
    return;
  }

  for (int handleIndex = 0; handleIndex < amountOfHandles; handleIndex++) {
    // Read all available protocols for this handle
    UINTN amountOfProtocols;
    EFI_GUID **availableProtocols;
    EFI_HANDLE deviceHandle = handleBuffer[handleIndex];
    status = BootServices->ProtocolsPerHandle(deviceHandle, &availableProtocols, &amountOfProtocols);
    if (status != EFI_SUCCESS) {
      continue;
    }

    for (int protocolIndex = 0; protocolIndex < amountOfProtocols; protocolIndex++) {
      // Leave only these handles that can handle BlockIOProtocol
      if (!isBlockIOProtocol(availableProtocols[protocolIndex])) {
        continue;
      }

      // Get IO interface for reading first block of partition
    	EFI_BLOCK_IO_PROTOCOL* ioInterface = NULL;
    	EFI_GUID blockIoProtocol = EFI_BLOCK_IO_PROTOCOL_GUID;
    	status = BootServices->HandleProtocol(deviceHandle, &blockIoProtocol, (void**)&ioInterface);
    	if (status != EFI_SUCCESS) {
    		return;
    	}

      // Leave only present media
      EFI_BLOCK_IO_MEDIA* media = ioInterface->Media;
      if (!media->MediaPresent) {
        continue;
      }

      // Prepare data for entry in partition list
      listOfPartitions[amountOfPartitions].deviceHandle = deviceHandle;
      listOfPartitions[amountOfPartitions].mediaId = media->MediaId;
      listOfPartitions[amountOfPartitions].blockSize = media->BlockSize;
      listOfPartitions[amountOfPartitions].size = media->BlockSize * media->LastBlock;

    	// Read first block of data from disk
    	UINTN bufferSize = media->BlockSize;
    	UINT8 buffer[bufferSize];
    	status = ioInterface->ReadBlocks(ioInterface, media->MediaId, 0, bufferSize, buffer);

      // Check partition type
      if (status != EFI_SUCCESS) {
        listOfPartitions[amountOfPartitions].type = UNKNOWN_PARTITION;
      } else if (isNTFS(buffer)) {
        listOfPartitions[amountOfPartitions].type = NTFS_PARTITION;
      } else if (isFAT(buffer)) {
        listOfPartitions[amountOfPartitions].type = FAT_PARTITION;
      } else {
        listOfPartitions[amountOfPartitions].type = UNKNOWN_PARTITION;
      }
      amountOfPartitions++;
  	}
  }
}

void printGptPartitions() {
  for (int i = 0; i < amountOfPartitions; i++) {
    UINT64 sizeInMB = listOfPartitions[i].size / 1024 / 1024;
    Print(L"Partition %d (Handle: 0x%08x) Block: %dB Size: %dMB Type: ", i, listOfPartitions[i].deviceHandle, listOfPartitions[i].blockSize, sizeInMB);
    if (listOfPartitions[i].type == NTFS_PARTITION) {
      Print(L"NTFS\n");
    } else if (listOfPartitions[i].type == FAT_PARTITION) {
      Print(L"FAT\n");
    } else {
      Print(L"Undefined\n");
    }
  }
}
