# 
# Created by Jakub Powierza, 2016
# 

# Constant environment variables
CPU=qemu32
BIOS=bin/uefi.bin
HDD_SYSTEM=fat
HDD_DIR=virtual-hdd

# Targets
all: develop run

develop:
	mkdir $(HDD_DIR)

run:
	qemu-system-x86_64 -cpu $(CPU) -bios $(BIOS) -hda $(HDD_SYSTEM):$(HDD_DIR)

clean:
	rm -rf $(HDD_DIR)

.PHONY: all develop run clean
