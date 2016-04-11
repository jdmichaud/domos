#include "protocol.h"

bool hammingw(uint64_t x) {
  x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
  x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits
  x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits
  x += x >>  8;  //put count of each 16 bits into their lowest 8 bits
  x += x >> 16;  //put count of each 32 bits into their lowest 8 bits
  x += x >> 32;  //put count of each 64 bits into their lowest 8 bits
  return x & 0x7f;
}

void create_packet(uint8_t sensor_type, uint8_t sensor_id,
                   uint32_t message_length, uint8_t *message,
                   packet_t *packet, uint8_t *messagerest) {
  bzero(packet, sizeof(packet_t));
  packet->spacket.magic = MAGIC_NUMBER;
  packet->spacket.stype = sensor_type;
  packet->spacket.sid = sensor_id;
  packet->spacket.mlength = message_length;
  packet->spacket.message = message[0];
  // Compute parity
  packet->parity = hammingw(packet.raw & 0x7FFFFF) & 1;
  // Set the rest of the message
  messagerest = NULL;
  if (message_length > 1)
    messagerest = &message[1];
}

int read_packet(uint64_t raw, packet_t *packet) {
  bzero(packet, sizeof(packet_t));
  packet->raw = raw;
  // Check maginc number
  if (packet->spacket.magic != MAGIC_NUMBER)
    return WRONG_MAGIC_ERROR
  // Check parity
  bool parity = hammingw(packet->raw & 0x7FFFFF) & 1;
  if (parity != packet->spacket.parity)
    return PARITY_ERROR;
  return 0;
}

int read_message(uint8_t *buffer, packet_t *packet, uint8_t *messagerest) {
  uint64_t header = buffer[0] << 48 | buffer[1] << 32 | buffer[2] << 16 | buffer[3];
  // Decode the header
  int ret = read_message(header, packet);
  if (ret) return ret;
  // If the message is longer than 1 octet, make it point to messagerest
  messagerest = NULL
  if (packet->spacket.mlength > 1)
    messagerest = &buffer[4];
  return 0;
}