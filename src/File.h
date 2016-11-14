/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#ifndef _FILE_H
#define _FILE_H

#include <efi.h>
#include <efilib.h>


void printCatalog(CHAR16* directoryPath, EFI_HANDLE ImageHandle, EFI_BOOT_SERVICES* BootServices);

#endif
