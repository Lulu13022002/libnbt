# libnbt

## Description

libnbt is a C/C++ single-header library for working with the NBT format used by Minecraft.

libnbt can:
* Read NBT files, both uncompressed and compressed (in both the zlib and Gzip formats).
* Create and modify in-memory NBT structures.
* Write in-memory NBT structures, both uncompressed and compressed (supporting both zlib and Gzip as with reading).
* Use the new long array tag added in Minecraft 1.12.
* Use the new short/float/double/string array tag recently added in NMSC.
* Pack 8, 16 and 32 bits (as booleans array) to int8_t, int16_t and int32_t with hardware acceleration (only for Intel since Pentium/Haswell for now).

libnbt does yet not provide support for:
* Reading .mca/.mcr files used for storing regions.
* Working with the SNBT format.

These are likely to be added in the future.

## Usage Instructions
libnbt is dependent on either [zlib](https://www.zlib.net/) or [miniz](https://github.com/richgel999/miniz). A copy of miniz is included in this repository.

If your program does not already use zlib or miniz, you will also need to compile `miniz.c`.

By default the packer is set to use the BMI2 instruction set provided by Intel.
You shouldn't use the BMI2 in `packer.c` (the macro `DYNAMIC_BMI2`) if you compile under an AMD processor.

### gcc or llvm/clang stuff:
When you use hardware acceleration with BMI2, you need to set `-mbmi2` or `-march=haswell` option to your compiler.

## Documentation
Documentation for the library is available [here](doc.md).

## Licence
libnbt  
Written in 2019 by IDidMakeThat

To the extent possible under law, the author(s) have dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along
with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

The original bigtest.nbt (made available here in three different formats) can be found [here](https://wiki.vg/NBT).
