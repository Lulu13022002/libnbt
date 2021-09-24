#include "packer.h"
#include <stdint.h>

#ifndef DYNAMIC_BMI2
  #if (((defined(__clang__) && __has_attribute(__target__)) \
      || (defined(__GNUC__) \
          && (__GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))) \
      && (defined(__x86_64__) || defined(_M_X86))) \
      && !defined(__BMI2__))
  #include <x86intrin.h>
  #define DYNAMIC_BMI2
  #endif
#endif

#define DllExport __declspec(dllexport)

DllExport bool* nbt_unpack_8bits(int8_t value, int size) {
    #ifdef DYNAMIC_BMI2
        return _pdep_u64(value, 0x0101010101010101ULL);
    #else
        #ifdef MULTIPLY_BIT
            bool* result = malloc(sizeof(bool*)*size);
            // on little-endian,  a[0] = (b>>7) & 1  like printing order
            unsigned long long int MAGIC = 0x8040201008040201ULL;  // for opposite order, byte-reverse this
            unsigned long long int MASK  = 0x8080808080808080ULL;
            uint64_t t = ((MAGIC*value) & MASK) >> 7;
            memcpy(result, &t, sizeof t);    // store 8 bytes without UB
            return result;
        #else
            bool* result = malloc(sizeof(bool)*size);

            for(int i = size - 1; i >= 0; i--)
            {
                result[i] = (bool)(value & 1);
                value >>= 1;
            }
            return result;
        #endif
    #endif
}

DllExport bool* nbt_unpack_16bits(int16_t value, int size) {
    #if defined(DYNAMIC_BMI2) || defined(MULTIPLY_BIT)
        return (nbt_unpack_8bits(value >> 8)) | (nbt_unpack_8bits(value >> 24 + 24));
    #else
        bool* result = malloc(sizeof(bool)*size);

        for(int i = size - 1; i >= 0; i--)
        {
            result[i] = (bool)(value & 1);
            value >>= 1;
        }
        return result;
    #endif
}

DllExport bool* nbt_unpack_32bits(int32_t value, int size) {
    #if defined(DYNAMIC_BMI2) || defined(MULTIPLY_BIT)
        return (nbt_unpack_8bits(value >> 24)) | (nbt_unpack_8bits(value >> 16 +  8)) |
               (nbt_unpack_8bits(value >> 8))  | (nbt_unpack_8bits(value >> 24 + 24));
    #else
        bool* result = malloc(sizeof(bool)*size);

        for(int i = size - 1; i >= 0; i--)
        {
            result[i] = (bool)(value & 1);
            value >>= 1;
        }
        return result;
    #endif
}

DllExport int8_t nbt_pack_8bits(bool* array, uint8_t size) {
    #ifdef DYNAMIC_BMI2
        return _pext_u64(*((uint64_t*)array), 0x0101010101010101ULL);
    #else
        #ifdef MULTIPLY_BIT
            int64_t t;
            memcpy(&t, array, sizeof t);         //  strict-aliasing & alignment safe load
            return 0x8040201008040201ULL*t >> 56;
            // bit order: a[0]<<7 | a[1]<<6 | ... | a[7]<<0  on little-endian
            // for a[0] => LSB, use 0x0102040810204080ULL    on little-endian
        #else
            int8_t result = 0;

            for(uint8_t i = 0; i < size; i++)
            {
                int8_t bit = (int8_t)*(array + i);
                result = (result << 1) | bit;
            }
            return result;
        #endif
    #endif
}

DllExport int16_t nbt_pack_16bits(bool* array, uint16_t size) {
    #if defined(DYNAMIC_BMI2) || defined(MULTIPLY_BIT)
        return (nbt_pack_8bits(array + 16) <<  8) | (nbt_pack_8bits(array + 24) <<  0);
    #else
        int16_t result = 0;

        for(uint16_t i = 0; i < size; i++)
        {
            int16_t bit = (int16_t)*(array + i);
            result = (result << 1) | bit;
        }
        return result;
    #endif
}

DllExport int32_t nbt_pack_32bits(bool* array, uint32_t size) {
    #if defined(DYNAMIC_BMI2) || defined(MULTIPLY_BIT)
        return (nbt_pack_8bits(array +  0) << 24) | (nbt_pack_8bits(array +  8) << 16) |
               (nbt_pack_8bits(array + 16) <<  8) | (nbt_pack_8bits(array + 24) <<  0);
    #else
        int32_t result = 0;

        for(uint32_t i = 0; i < size; i++)
        {
            int32_t bit = (int32_t)*(array + i);
            result = (result << 1) | bit;
        }
        return result;
    #endif
}
