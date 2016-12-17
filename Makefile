#
# IntelGalileoOS
# Created by: Jakub Powierza
#

EDKII_BUILD_COMMAND = build
EDKII_BUILD_DIRECTORY = Build/IntelGalileoOS/DEBUG_GCC5/IA32

CURRENT_DIRECTORY = "$(shell pwd)"
BUILD_DIRECTORY = build
EFI_FILE = IntelGalileoOS.efi
OVMF_IMAGE = bin/OVMF.fd

all: build

develop:
	mkdir -p $(BUILD_DIRECTORY)

build: develop
	cd .. && $(EDKII_BUILD_COMMAND) && cp ./$(EDKII_BUILD_DIRECTORY)/$(EFI_FILE) $(CURRENT_DIRECTORY)/$(BUILD_DIRECTORY)/$(EFI_FILE)

run:
	qemu-system-i386 -bios $(OVMF_IMAGE) -hda fat:$(BUILD_DIRECTORY)

clean:
	rm -rf $(BUILD_DIRECTORY)

.PHONY: all build develop run clean
