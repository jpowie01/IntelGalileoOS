/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#ifndef _GPT_PARTITIONS_H
#define _GPT_PARTITIONS_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/BlockIo.h>

#define MAX_AMOUNT_OF_PARTITIONS  128

#define FAT_PARTITION       0x01
#define NTFS_PARTITION      0x02
#define UNKNOWN_PARTITION   0x03

typedef struct {
	EFI_HANDLE deviceHandle;
	UINT32 mediaId;
	UINTN blockSize;
	UINT64 size;
	UINT8 type;
} GPTPartition;

extern UINTN amountOfPartitions;
extern GPTPartition listOfPartitions[MAX_AMOUNT_OF_PARTITIONS];

void prepareListOfGptPartitions(EFI_BOOT_SERVICES* BootServices);
void printGptPartitions();

#endif
