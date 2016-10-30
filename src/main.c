/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include <efi.h>
#include <efilib.h>

#include "PageDirectory.h"
#include "AssemblyHelper.h"


EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    Print(L"Starting...\n");
    Print(L"CR0: %x\n", getCR0());
    Print(L"CR3: %x\n", getCR3());
    createPageDirectory(PAGE_DIRECTORY_ADDRESS);
    setCR3((void*)PAGE_DIRECTORY_ADDRESS);
    Print(L"Everything is prepared.\n");
    Print(L"Enabling paging...\n");
    enablePaging();
    Print(L"CR0: %x\n", getCR0());
    Print(L"CR3: %x\n", getCR3());
    Print(L"Done!\n");
    Print(L"Should work like a charm!\n");

    return EFI_SUCCESS;
}
