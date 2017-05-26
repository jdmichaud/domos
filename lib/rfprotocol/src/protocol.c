#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "bitop.h"
#include "protocol.h"
// For debugging
// uint32_t t;
// printf("%" PRIu32 "\n", t);
#include <inttypes.h>

const uint64_t m1  = 0x5555555555555555; //binary: 0101...
const uint64_t m2  = 0x3333333333333333; //binary: 00110011..
const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...

uint8_t hammingw(uint64_t x) {
  x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
  x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits
  x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits
  x += x >>  8;  //put count of each 16 bits into their lowest 8 bits
  x += x >> 16;  //put count of each 32 bits into their lowest 8 bits
  x += x >> 32;  //put count of each 64 bits into their lowest 8 bits
  return x & 0x7f;
}

void create_packet(uint8_t sensor_type, uint8_t sensor_id,
                   bool battery_indicator, uint8_t message,
                   packet_t *packet) {
  memset(packet, 0, sizeof(packet_t));
  pack(MAGIC_NUMBER,  MAGIC_NUMBER_SIZE,  (uint8_t *) packet, MAGIC_NUMBER_OFFSET);
  pack(battery_indicator ? 1 : 0,
                      BATT_REPL_SIZE,     (uint8_t *) packet, BATT_REPL_OFFSET);
  pack(sensor_type,   SENSOR_TYPE_SIZE,   (uint8_t *) packet, SENSOR_TYPE_OFFSET);
  pack(sensor_id,     SENSOR_ID_SIZE,     (uint8_t *) packet, SENSOR_ID_OFFSET);
  pack(message,       MESSAGE_SIZE,       (uint8_t *) packet, MESSAGE_OFFSET);
  // Compute parity
#ifdef LITTLE_ENDIAN
  pack(hammingw(*packet & 0xFFFFFFFFFFFFFE00) & 1, PARITY_SIZE,
                (uint8_t *) packet, PARITY_OFFSET);
#else
  pack(hammingw(*packet & 0x7FFFFFFFFFFFFF) & 1, PARITY_SIZE,
                (uint8_t *) packet, PARITY_OFFSET);
#endif
}

int read_packet(packet_t packet, packet_s *spacket) {
  uint8_t magic_number = 0;
  uint8_t parity = 0;
  uint8_t battery_indicator = 0;
  memset(spacket, 0, sizeof(packet_s));
  unpackc(&magic_number,       MAGIC_NUMBER_SIZE,  (uint8_t *) &packet, MAGIC_NUMBER_OFFSET);
  unpackc(&parity,             PARITY_SIZE,        (uint8_t *) &packet, PARITY_OFFSET);
  unpackc(&battery_indicator,  BATT_REPL_SIZE,     (uint8_t *) &packet, BATT_REPL_OFFSET);
  spacket->battery_indicator = battery_indicator == 1 ? true : false;
  unpackc(&(spacket->stype),   SENSOR_TYPE_SIZE,   (uint8_t *) &packet, SENSOR_TYPE_OFFSET);
  unpackc(&(spacket->sid),     SENSOR_ID_SIZE,     (uint8_t *) &packet, SENSOR_ID_OFFSET);
  unpackc(&(spacket->message), MESSAGE_SIZE,       (uint8_t *) &packet, MESSAGE_OFFSET);
  // Check magic number
  if (magic_number != MAGIC_NUMBER)
    return WRONG_MAGIC_ERROR;
  // Check parity
#ifdef LITTLE_ENDIAN
  bool computed_parity = hammingw(packet & 0xFFFFFFFFFFFF7F00) & 1;
#else
  bool computed_parity = hammingw(packet & 0x7FFFFFFFFFFFFF) & 1;
#endif
  if (computed_parity != parity)
    return PARITY_ERROR;
  return 0;
}
