#include <math.h>
#include <stdio.h>
#include "bitop.h"

uint32_t pack(uint32_t value, uint8_t size, uint8_t *buffer, uint32_t offset) {
  uint8_t new_offset = offset;
  while (size > 0) { // As long as there are some bits to store
    // Point to the next char with empty bits in the buffer
    uint8_t *p = buffer + (new_offset + 1) / (sizeof (char) * CHAR_BIT);
    uint32_t local_offset = new_offset % (sizeof (char) * CHAR_BIT);
    uint8_t shift = 0;
    uint8_t shifted_value = 0;
    uint8_t written_bits = 0;
    if (size >= CHAR_BIT - local_offset) {
      /* If the size of the value is greater or equal than the remaining empty
       * bit in the next byte, shift to the right
       * value:  .... ....  .... ....  .... ..xx  xxxx xxxx
       * buffer:                                  yyyy y...
       *                                      --  ---- >xxx  xxxx xxx.
       *                                      12  3456 7
       * Shall shift right by 7
       */
      written_bits = CHAR_BIT - local_offset;
      shift = size - written_bits;
      shifted_value = value >> shift;
    } else {
      /* If the size of the value is less than the remaining empty bit in the
       * next byte, shift to the left
       * value:  .... ....  .... ....  .... ....  .... ..xx
       * buffer:                                  yyyy y...
       *                                                xx<
       *                                                  1
       * Shall shift left by 1
       */
      written_bits = size;
      shift = CHAR_BIT - local_offset - size;
      shifted_value = value << shift;
    }
    // Write the bits into the buffer
    p[0] = p[0] | shifted_value;
    size -= written_bits;
    new_offset += written_bits;
    /*
     * Mask the remaining bits
     * value: .... ....  .... ....  .... ..**  xxxx xxxx
     *                                     ^^-- already written
     * remaining size: 8 bits
     * 2^8  : 0000 0000  0000 0000  0000 0001  0000 0000
     *  -1  : 0000 0000  0000 0000  0000 0000  1111 1111
     */
    value = value & ((1 << size) - 1);
  }
  return new_offset;
}

uint32_t unpack(uint64_t *value, uint8_t size,
                uint8_t *buffer, uint32_t offset) {
  *value = 0u;
  /*
   * buffer: .... ....  .... ....  .... ..XX XXXX  XXX.
   * offset: 22                           ^-- offset
   * size  : 9
   */
   uint32_t new_offset = offset;
   while (size > 0) {
    /*
     * buffer: .... ....  .... ....  .... ..XX XXXX  XXX.
     * v     :                            ..XX XXXX
     * size  : 9
     */
    uint8_t  v = *(buffer + ((new_offset + 1) / (sizeof (char) * CHAR_BIT)));
    uint32_t local_offset = new_offset % (sizeof (char) * CHAR_BIT);
    uint8_t written_bits = 0;
    if (size >= CHAR_BIT) {
      written_bits = CHAR_BIT - local_offset;
    } else {
      written_bits = size;
      v >>= CHAR_BIT - local_offset - size;
    }
    /*
     * buffer:     .... ....  .... ....  .... ...X XXXX  ....
     * v     :                                ...X XXXX
     * write_mask: 0000 0000  0000 0000  0000 0001 1111
     * size  : 9
     */
    uint64_t write_mask = (1 << written_bits) - 1;
    /*
     * v     :                                             ..XX XXXX
     * write_mask:                                         0011 1111
     * value : .... ....  .... ....  .... ....  .... ....  YYXX XXXX
     */
    *value |= (v & write_mask);
    size -= written_bits;
    *value <<= size < 8 ? size : 8;
    new_offset += written_bits;
  }
  return new_offset;
}

uint32_t unpackc(uint8_t *value, uint8_t size,
                 uint8_t *buffer, uint32_t offset) {
  uint64_t value64 = 0u;
  uint32_t ret = unpack(&value64, size, buffer, offset);
  *value = (uint8_t) value64;
  return ret;
}

uint32_t unpacki(uint32_t *value, uint8_t size,
                 uint8_t *buffer, uint32_t offset) {
  uint64_t value64 = 0u;
  uint32_t ret = unpack(&value64, size, buffer, offset);
  *value = (uint32_t) value64;
  return ret;
}
