Intel Galileo OS
================

This is repository for basic Operating System written for Intel Galileo (i586).

### Requirements

All you need is:
- make
- qemu
- EDKII environment

### Prepare your environment

To start the development precess you've got to prepare EDKII environment using instructions from [documentation](https://wiki.ubuntu.com/UEFI/EDK2#Initial_set_up). *Please follow ONLY the section named "Initial set up"!* While preparing build target setup please use these entries:

```
ACTIVE_PLATFORM = IntelGalileoOS/IntelGalileoOS.dsc
TOOL_CHAIN_TAG  = GCC5
TARGET_ARCH     = IA32
```

After that, clone this repository into your EDKII root directory and enter it. That's all!

### Build & run

Once you've done all preparations, you'll be able to build everything with `make build` and run your OS in QEMU emulator by `make run`.
