/*
    IntelGalileoOS
    Created by: Jakub Powierza
*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>

#include "paging/PageDirectory.h"
#include "paging/AssemblyHelper.h"
#include "filesystem/fat/File.h"
#include "filesystem/ntfs/File.h"
#include "filesystem/partitions/Gpt.h"
#include "keyboard/Keyboard.h"


EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	// Clear the screen and turn off watchdog
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);

	// Initialize and print header
	Print(L"\n Intel Galileo OS\n");
	Print(L" Created by: Jakub Powierza\n");
	Print(L"============================\n");

	// Settings
	UINT8 paging = PAGING_DISABLED;

	// Generate list of partitions for disk operations
	prepareListOfGptPartitions(SystemTable->BootServices);

	// Never ending loop of reading commands from keyboard
	CHAR16 command[MAX_STRING_FROM_KEYBOARD];
	do {
		// Prompt key
		Print(L" > ");

		// Read command
		readStringFromKeyboard(SystemTable, command);

		// Print registers
		if (StrCmp(command, L"registers") == 0) {
			Print(L"CR0: 0x%08x\nCR3: 0x%08x\n", getCR0(), getCR3());
			continue;
		}

		if (StrCmp(command, L"turn_on_paging") == 0) {
			// Prepare Page Directory with all Page Tables
			Print(L"Creating all tables... ");
			createPageDirectory(PAGE_DIRECTORY_ADDRESS);
			setCR3((void*)PAGE_DIRECTORY_ADDRESS);
			Print(L"Done.\n");

			// Turn on paging
			Print(L"Enabling paging... ");
			enablePaging();
			paging = PAGING_ENABLED;
			Print(L"Done.\n");
			Print(L"Paging enabled properly!\n");
			continue;
		}

		if (StrCmp(command, L"check_paging") == 0) {
			// Make sure that paging is enabled
			if (paging == PAGING_DISABLED) {
				Print(L"Turn on paging first!\n");
				continue;
			}

			// Try to switch two physical addresses
			Print(L"Physical address for linear address (0xE0000000): 0x%08x\n", getPhysicalAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xE0000000));
			Print(L"Physical address for linear address (0xD0000000): 0x%08x\n", getPhysicalAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xD0000000));
			Print(L"Mapping linear address (0xE0000000) to physical address (0xD0000000)...\n");
			mapLinearAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xD0000000, (void*)0xE0000000, EMPTY_TABLE_ENTRY_FLAGS);
			Print(L"Mapping linear address (0xD0000000) to physical address (0xE0000000)...\n");
			mapLinearAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xE0000000, (void*)0xD0000000, EMPTY_TABLE_ENTRY_FLAGS);
			Print(L"Physical address for linear address (0xE0000000): 0x%08x\n", getPhysicalAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xE0000000));
			Print(L"Physical address for linear address (0xD0000000): 0x%08x\n", getPhysicalAddress(PAGE_DIRECTORY_ADDRESS, (void*)0xD0000000));
			continue;
		}

		// List all GPT partitions
		if (StrCmp(command, L"partitions") == 0) {
			printGptPartitions();
			continue;
		}

		// List all entries in root directory of partition
		if (StrCmp(command, L"ls") == 0) {
			Print(L"Partition number: ");
			readStringFromKeyboard(SystemTable, command);
			UINT8 partitionNumber = command[0] - 48; // FIXME: Only one digit works for now...
			if (partitionNumber < 0 || partitionNumber >= amountOfPartitions) {
				Print(L"Invalid partition number.\n");
				continue;
			}

			// Get partition and print any directory
			GPTPartition partition = listOfPartitions[partitionNumber];
			if (partition.type == FAT_PARTITION) {
				Print(L"Enter directory path: ");
				readStringFromKeyboard(SystemTable, command);
				printFATCatalog(command, partition.deviceHandle, SystemTable->BootServices);
			} else if (partition.type == NTFS_PARTITION) {
				printNTFSCatalog(L"\\", partition.deviceHandle, SystemTable->BootServices);
			} else {
				Print(L"Unknown partition type.\n");
			}
			continue;
		}

		// Clear the screen
		if (StrCmp(command, L"clear") == 0) {
			SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
			continue;
		}

		// Print author of this OS
		if (StrCmp(command, L"author") == 0) {
			Print(L"Jakub Powierza (github.com/jpowie01)\n");
			continue;
		}

		// Show help with all commands
		if (StrCmp(command, L"help") == 0 || StrCmp(command, L"?") == 0) {
			Print(L"\n All available commands\n");
			Print(L"-------------------------------------------------------\n");
			Print(L" registers - Print CR0 & CR3 registers\n");
			Print(L" turn_on_paging - Create all tables & turns on paging\n");
			Print(L" check_paging - Run quick test for paging\n");
			Print(L" partitions - List all GPT partitions available\n");
			Print(L" ls - List files from root directory\n");
			Print(L" clear - Clear screen\n");
			Print(L" author - Print author\n");
			Print(L" help - List all commands\n");
			Print(L" exit - Exit from OS\n");
			continue;
		}

		// Exit from OS
		if (StrCmp(command, L"exit") == 0) {
			break;
		}

		// Information about invalid command
		Print(L"Invalid command. Please try again!\n");
	} while (1);

	// End of program
	Print(L"Exit...\n");
	return EFI_SUCCESS;
}
