#include <stdint.h>
#include <stdbool.h>

#define MAGIC_NUMBER      178

#define WRONG_MAGIC_ERROR 1
#define PARITY_ERROR      2

/*! \brief Packet structure use as the first element sent in a message
 *
 *  The protocol consist of a header packet, represented by the packet_t
 *  structure and a buffer containing the content of the message.
 *  The header packet contains:
 *  nb of bits  name               comment
 *  ----------------------------------------------------------------------------
 *  8           magic number       Defined in this file, its value is always 
 *                                 178. The 433Mhz band is pretty busy so we 
 *                                 will be looking at this value to identify
 *                                 one of our messages.
 *  1           parity bit         Used to detect integrity errors in the
 *                                 packet structure.
 *                                 1: the number of 1 in the fields stype, sid,
 *                                    mlength and message is odd,
 *                                 0: otherwise.
 *                                 The parity bit does not include the byte of
 *                                 message after the first one (after the one
 *                                 stored in the header packet)
 *  8           sensor type        8 bits integer representing the type of
 *                                 sensor. See sensor_type.h
 *  8           sensor id          8 bits integer representing the identifier of
 *                                 the sensor.
 *  31          length of the      The total length of the message following the
 *              message            header packet
 *  8           first octet of     The first octet of the message.
 *              the message
 */
 union {
  uint64_t raw;
  struct {
    uint8_t magic     : 8;  // magic number (shoule be MAGIC_NUMBER)
    uint8_t parity    : 1;  // 1 if number of 1 in following fields is odd
    uint8_t stype     : 8;  // type of sensor (0..255)
    uint8_t sid       : 8;  // sensor id (0..255). 0 in simple sensors
    uint32_t mlength  : 31; // length in octets of the message
    uint16_t message  : 8;  // message (or head of the meessage if length > 1)
  } spacket;
} packet_t;

/*! \brief Hamming weigh computation on 64 bits.
 *
 *  Compute the number of 1s in the provided 64 bits parameter.
 */
uint8_t hammingw(uint64_t val);

/*! \brief From various information, create a header packet.
 *
 *  Takes the packet information and the message and provide a header packet and
 *  a pointer of the rest of the message.
 */
void create_packet(uint8_t sensor_type, uint8_t sensor_id, 
                   uint32_t message_length, uint8_t *message,
                   packet_t packet, uint8_t *messagerest);

/*! \brief Convert 64 bit to packet structure.
 *
 *  Convert a raw 64 bits integer to a packet_t structure, checking the magic
 *  number and the parity bit.
 *  Returns:
 *  If the magic number is wrong, returns WRONG_MAGIC_ERROR
 *  If the parity is incorrect, returns PARITY_ERROR
 */
int read_packet(uint64_t raw, packet_t *packet);

/*! \brief Convert a buffer to a header packet and a buffer.
 *
 *  Take a buffer and produce a packet and the rest of the message in
 *  messagerest
 *  Returns:
 *  If the magic number is wrong, returns WRONG_MAGIC_ERROR
 *  If the parity is incorrect, returns PARITY_ERROR
 */
int read_message(uint8_t *buffer, packet_t *packet, uint8_t *messagerest);

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