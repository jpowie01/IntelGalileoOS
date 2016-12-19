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
#include <Protocol/SimpleFileSystem.h>

void printNTFSCatalog(CHAR16* directoryPath, EFI_HANDLE ImageHandle, EFI_BOOT_SERVICES* BootServices);

#endif
