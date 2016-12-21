/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#ifndef _FAT_FILE_H
#define _FAT_FILE_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Guid/FileInfo.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>

void printFATCatalog(CHAR16* directoryPath, EFI_HANDLE deviceHandle, EFI_BOOT_SERVICES* BootServices);

#endif
