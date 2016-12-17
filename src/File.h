/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#ifndef _FILE_H
#define _FILE_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Guid/FileInfo.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>


void printCatalog(CHAR16* directoryPath, EFI_HANDLE ImageHandle, EFI_BOOT_SERVICES* BootServices);

#endif
