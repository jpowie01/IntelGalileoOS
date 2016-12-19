/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <Uefi.h>
#include <Library/UefiLib.h>

#define MAX_STRING_FROM_KEYBOARD    512

void readStringFromKeyboard(EFI_SYSTEM_TABLE *SystemTable, CHAR16* string);

#endif
