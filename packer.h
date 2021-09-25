#ifndef PACKER_H
#define PACKER_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#ifndef NBT_NO_STDINT
#include <stdint.h>
#endif

#define DllImport __declspec(dllimport)

DllImport bool* nbt_unpack_8bits(int8_t value, int size);
DllImport bool* nbt_unpack_16bits(int16_t value, int size);
DllImport bool* nbt_unpack_32bits(int32_t value, int size);

DllImport int8_t nbt_pack_8bits(bool* array, uint8_t size);
DllImport int16_t nbt_pack_16bits(bool* array, uint16_t size);
DllImport int32_t nbt_pack_32bits(bool* array, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
