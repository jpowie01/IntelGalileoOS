/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#ifndef _NTFS_FILE_H
#define _NTFS_FILE_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Guid/FileInfo.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/BlockIo.h>

void printNTFSCatalog(CHAR16* directoryPath, EFI_HANDLE deviceHandle, EFI_BOOT_SERVICES* BootServices);

#endif
