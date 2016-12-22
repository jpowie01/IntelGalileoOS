Intel Galileo OS
================

This is repository for basic Operating System written for Intel Galileo (i586).

### Requirements

All you need is:
- Make,
- QEMU,
- EDKII environment (with dependencies).

### Prepare your environment

To start the development precess you've got to prepare EDKII environment using instructions from [documentation](https://wiki.ubuntu.com/UEFI/EDK2#Initial_set_up). **Please follow ONLY the section named "Initial set up"!** While preparing build target setup please use these entries:

```
ACTIVE_PLATFORM = IntelGalileoOS/IntelGalileoOS.dsc
TOOL_CHAIN_TAG  = GCC5
TARGET_ARCH     = IA32
```

After that, clone this repository into your EDKII root directory and enter it. That's all!

### Build & run

Once you've done all preparations, you'll be able to build everything with `make build` and run your OS in QEMU emulator by `make run`.

### Prepare testing image

You can run this OS with above instructions but if you want to implement eg. NTFS driver, you need to prepare testing image. You can use any tool for partitioning you want. But you've got to remember that:
- you need to create an image with GPT (GUID Partition Table),
- you can create as many partition as you want (eg. two - one with FAT and second one with NTFS),
- place your image inside `image` folder and name it `OS.img`.
Once you've prepared your image you can run QEMU with it by typing `make runfromdisk`. There is also a way to combine `build` folder with your image by running `make runwithdisk`. It will start QEMU with .efi app from `build` directory with your image mounted besides.

### Create SD card for Intel Galileo

If you want to run this OS on Intel Galileo, you've got to prepare an SD card. You've got to do similar steps as above for creating a testing image. But you've got to remember that you need to have at least one FAT partition with your .efi app! Instead, you won't be able to boot into the OS (UEFI can boot only from FAT partitions)!
