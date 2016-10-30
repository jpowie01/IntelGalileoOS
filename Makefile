#
# IntelGalileoOS
# Created by: Jakub Powierza
#

ARCH            = $(shell uname -m | sed s,i[3456789]86,ia32,)

OBJS            = src/main.o src/PageDirectory.o src/AssemblyHelper.o
HEADERS			= src/PageDirectory.h src/AssemblyHelper.h
TARGET          = build/main.efi

EFIINC          = lib/gnu-efi/inc
EFIINCS         = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
LIB             = lib/gnu-efi/ia32/lib
EFILIB          = lib/gnu-efi/gnuefi
EFI_CRT_OBJS    = $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS         = $(EFILIB)/elf_$(ARCH)_efi.lds

CFLAGS          = $(EFIINCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Wall -masm=intel

ifeq ($(ARCH),x86_64)
	CFLAGS += -DEFI_FUNCTION_WRAPPER
endif

LDFLAGS         = -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L $(EFILIB) -L $(LIB) $(EFI_CRT_OBJS)

all: $(TARGET)

build: $(TARGET)

develop:
	mkdir build

src/%.so: $(OBJS) $(HEADERS)
	ld $(LDFLAGS) $(OBJS) -o $@ -l:libefi.a -l:libgnuefi.a

build/%.efi: src/%.so
	objcopy -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-$(ARCH) $^ $@

run:
	qemu-system-i386 -bios bin/OVMF.fd -hda fat:build

clean:
	rm -rf build
	rm -f *.o
	rm -f *.so

.PHONY: all build develop run clean
