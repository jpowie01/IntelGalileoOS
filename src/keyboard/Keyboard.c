/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include "Keyboard.h"

void readStringFromKeyboard(EFI_SYSTEM_TABLE *SystemTable, CHAR16* string) {
  EFI_INPUT_KEY singleKey;
  CHAR16 character[2] = { 0, 0 };  // Unicode character that we will be printing on screen
  UINT16 index = 0;

  // Read keys from keyboard until end of available characters - 1 (leave space for 0 at the end)
  do {
    // Print cursor character
    character[0] = '_';
    SystemTable->ConOut->OutputString(SystemTable->ConOut, character);

    // Try to read until success
    while(SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &singleKey) != EFI_SUCCESS);

    // End on "End of Line"
    if (singleKey.UnicodeChar == '\r' || singleKey.UnicodeChar == '\n') {
      break;
    }

    // Remove cursor character
    character[0] = '\b';
    SystemTable->ConOut->OutputString(SystemTable->ConOut, character);

    // Be sure that user won't backspace too much
    if (singleKey.UnicodeChar == '\b') {
      // Remove last sign if needed (both from result string and from screen)
      if (index > 0) {
        index--;
        character[0] = singleKey.UnicodeChar;
        SystemTable->ConOut->OutputString(SystemTable->ConOut, character);
      }
      continue;
    }

    // Print key read from keyboard
    character[0] = singleKey.UnicodeChar;
    SystemTable->ConOut->OutputString(SystemTable->ConOut, character);

    // Add key into result string
    string[index++] = singleKey.UnicodeChar;
  } while (index < MAX_STRING_FROM_KEYBOARD - 1);

  // Each string should end with 0
  string[index] = 0;

  // Remove cursor character
  character[0] = '\b';
  SystemTable->ConOut->OutputString(SystemTable->ConOut, character);

  // Print "End of Line"
  character[0] = '\n';
  SystemTable->ConOut->OutputString(SystemTable->ConOut, character);

  // Print "Carriage Return"
  character[0] = '\r';
  SystemTable->ConOut->OutputString(SystemTable->ConOut, character);
}
