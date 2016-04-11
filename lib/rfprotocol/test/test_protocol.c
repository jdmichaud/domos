#include "protocol.h"

test_hammingw() {
  EXPECT_EQ(0, hammingw(0));
  EXPECT_EQ(1, hammingw(1));
  EXPECT_EQ(1, hammingw(2));
  EXPECT_EQ(2, hammingw(3));
  EXPECT_EQ(3, hammingw(42));
  EXPECT_EQ(5, hammingw(666));
  EXPECT_EQ(33, hammingw(5555555555555555555));
}

test_create_packet() {
  uint8_t message[1] = { 170 };
  packet_t packet;
  uint8_t *rest = &message;
  create_packet(12, 248, 1, message, &packet, rest);
  EXPECT_EQ(12, packet.spacket.stype);
  EXPECT_EQ(248, packet.spacket.sid);
  EXPECT_EQ(1, packet.spacket.mlength);
  EXPECT_EQ(170, packet.spacket.message);
  EXPECT_EQ(NULL, rest);
}

test_create_packet_long_message() {
  uint8_t message[3] = { 170, 42, 254 };
  packet_t packet;
  uint8_t *rest = &message;
  create_packet(12, 248, 3, message, &packet, rest);
  EXPECT_EQ(170, packet.spacket.message);
  EXPECT_EQ(&message[1], rest);
}

test_read_packet() {
  packet_t ipacket;
  ipacket.spacket.magic = MAGIC_NUMBER;
  ipacket.spacket.stype = 52; // 3 bits
  ipacket.spacket.sid = 196; // 3 bits
  ipacket.spacket.mlength = 1; // 1 bit
  ipacket.spacket.message = 28; // 3 bits
  ipacket.spacket.parity = 1; // 3 + 3 + 1 + 3 = 10 -> even number of 1
  packet_t opacket;
  int ret = read_packet(ipacket->raw, &opacket);
  EXPECT_EQ(0, ret);
  EXPECT_EQ(ipacket.spacket.magic, opacket.spacket.magic);
  EXPECT_EQ(ipacket.spacket.parity, opacket.spacket.parity);
  EXPECT_EQ(ipacket.spacket.stype, opacket.spacket.stype);
  EXPECT_EQ(ipacket.spacket.sid, opacket.spacket.sid);
  EXPECT_EQ(ipacket.spacket.mlength, opacket.spacket.mlength);
  EXPECT_EQ(ipacket.spacket.message, opacket.spacket.message);
}

test_read_packet_wrong_magic_number() {
  packet_t ipacket;
  ipacket.spacket.magic = 42;
  ipacket.spacket.stype = 52; // 3 bits
  ipacket.spacket.sid = 196; // 3 bits
  ipacket.spacket.mlength = 1; // 1 bit
  ipacket.spacket.message = 28; // 3 bits
  ipacket.spacket.parity = 1; // 3 + 3 + 1 + 3 = 10 -> even number of 1
  packet_t opacket;
  int ret = read_packet(ipacket->raw, &opacket);
  EXPECT_EQ(WRONG_MAGIC_ERROR, ret);
}

test_read_packet_parity_error() {
  packet_t ipacket;
  ipacket.spacket.magic = MAGIC_NUMBER;
  ipacket.spacket.stype = 51; // 3 bits
  ipacket.spacket.sid = 196; // 4 bits
  ipacket.spacket.mlength = 1; // 1 bit
  ipacket.spacket.message = 28; // 3 bits
  ipacket.spacket.parity = 1; // 4 + 3 + 1 + 3 = 11 -> odd number of 1
  packet_t opacket;
  int ret = read_packet(ipacket->raw, &opacket);
  EXPECT_EQ(PARITY_ERROR, ret);
}

test_read_message() {
  // Prepare input
  uint8_t *buffer = new uint8_t[sizeof(uint8_t * 5)];
  packet_t ipacket;
  ipacket.spacket.magic = MAGIC_NUMBER;
  ipacket.spacket.stype = 52; // 3 bits
  ipacket.spacket.sid = 196; // 3 bits
  ipacket.spacket.mlength = 2; // 1 bit
  ipacket.spacket.message = 28; // 3 bits
  ipacket.spacket.parity = 1; // 3 + 3 + 1 + 3 = 10 -> even number of 1
  // Copy the header into the buffer and append an additional ictet
  memcpy(buffer, ipacket, sizeof(uint8_t));
  buffer[5] = 69;
  // Observed outcome
  packet_t opacket;
  uint8_t *messagerest;
  int ret = read_message(buffer, &opacket, messagerest);
  EXPECT_EQ(0, ret);
  EXPECT_EQ(ipacket.spacket.magic, opacket.spacket.magic);
  EXPECT_EQ(ipacket.spacket.parity, opacket.spacket.parity);
  EXPECT_EQ(ipacket.spacket.stype, opacket.spacket.stype);
  EXPECT_EQ(ipacket.spacket.sid, opacket.spacket.sid);
  EXPECT_EQ(ipacket.spacket.mlength, opacket.spacket.mlength);
  EXPECT_EQ(ipacket.spacket.message, opacket.spacket.message);
  EXPECT_EQ(buffer[5], messagerest[0])
  delete[] buffer;
}