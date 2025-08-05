
## Bare mininum C++ project template for K1921VG015

IDE - VSCode
Build system - SCons
Target board - NIIET v.2

## Prerequisites
- VSCode (optional)
- Python + SCons
- riscv-none-elf-gcc - https://github.com/riscv-collab/riscv-gnu-toolchain/releases
- OpenOCD with Flash friver for K1921VG015 - https://gitflic.ru/project/niiet/k1921vkx_sdk/file?file=tools%2Fopenocd&branch=master or https://github.com/DCVostok/openocd-k1921vk/releases/tag/v0.12.0-k1921vk

## Build

- configure appropriate build tool paths in SConscript and .vscode/launch.json
- 'scons' command for build and flash (Ctrl+Shif+B in VSCode)
- F5 for dedug in VSCode
