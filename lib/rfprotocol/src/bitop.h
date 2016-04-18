#ifndef __BITOP_H__
#define __BITOP_H__

#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/*
 * Determine endianess
 */
#ifndef LITTLE_ENDIAN
# if defined(__BYTE_ORDER) || defined(BYTE_ORDER)
#  if (__BYTE_ORDER == __LITTLE_ENDIAN) || (BYTE_ORDER == LITTLE_ENDIAN)
#   define LITTLE_ENDIAN
#  else
#   if defined (i386) || defined (__i386__) || defined (_M_IX86) || \
       defined (vax) || defined (__alpha)
#    define LITTLE_ENDIAN
#   endif
#  endif
# endif
#endif

/*
 * Packs the size least significatn bit of value into the buffer
 * Returns the new offset
 * Before:
 *  buffer: yyy. ....  .... ....  .... etc.
 *             ^-- offset
 *  value:  .... ....  .... ....  .... ..xx  xxxx xxxx
 *  size:   10
 * After:
 *  buffer: yyyx xxxx  xxxx x...  .... etc.
 *                          ^-- offset
 */
uint32_t pack(uint32_t value, uint8_t size, uint8_t *buffer, uint32_t offset);

/*
 * Set value to the value read of size size at offset offset in buffer buffer
 */
uint32_t unpack(uint64_t *value, uint8_t size,
                uint8_t *buffer, uint32_t offset);
uint32_t unpackc(uint8_t *value, uint8_t size,
                 uint8_t *buffer, uint32_t offset);
uint32_t unpacki(uint32_t *value, uint8_t size,
                 uint8_t *buffer, uint32_t offset);

#ifdef __cplusplus
}
#endif

#endif //#ifndef __BITOP_H__
