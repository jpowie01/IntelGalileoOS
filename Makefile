#
# IntelGalileoOS
# Created by: Jakub Powierza
#

EDKII_BUILD_COMMAND = build
EDKII_BUILD_DIRECTORY = Build/IntelGalileoOS/DEBUG_GCC5/IA32

CURRENT_DIRECTORY = "$(shell pwd)"
BUILD_DIRECTORY = build
BOOT_DIRECTORY = EFI/BOOT
SOURCE_EFI_FILE = IntelGalileoOS.efi
DESTINATION_EFI_FILE = BOOTIA32.efi
OVMF_IMAGE = bin/OVMF.fd
DISK_IMAGE = image/OS.img

all: build

develop:
	mkdir -p $(BUILD_DIRECTORY)/$(BOOT_DIRECTORY)

build: develop
	cd .. && $(EDKII_BUILD_COMMAND) && cp ./$(EDKII_BUILD_DIRECTORY)/$(SOURCE_EFI_FILE) $(CURRENT_DIRECTORY)/$(BUILD_DIRECTORY)/$(BOOT_DIRECTORY)/$(DESTINATION_EFI_FILE)

run:
	qemu-system-i386 -bios $(OVMF_IMAGE) -hda fat:$(BUILD_DIRECTORY)

runfromdisk:
	qemu-system-i386 -bios $(OVMF_IMAGE) -drive format=raw,file=$(DISK_IMAGE)

runwithdisk:
	qemu-system-i386 -bios $(OVMF_IMAGE) -hda fat:$(BUILD_DIRECTORY) -drive format=raw,file=$(DISK_IMAGE) -boot d

clean:
	rm -rf $(BUILD_DIRECTORY)
	cd .. && rm -rf ./$(EDKII_BUILD_DIRECTORY)

.PHONY: all build develop run runfromdisk runwithdisk clean
