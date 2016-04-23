#include <stdio.h>
#include <stddef.h>
#include <strings.h>
#include "gtest/gtest.h"
#include "protocol.h"

char *number_to_binary(uint8_t x)
{
    static char b[9];
    b[8] = '\0';

    uint8_t z;
    int w = 0;
    for (z = 128; w < 8; z >>= 1, ++w)
    {
        b[w] = ((x & z) == z) ? '1' : '0';
    }

    return b;
}

char *number_to_binary_64(uint64_t x)
{
    static char b[65];
    b[64] = '\0';

    uint64_t z;
    int w = 0;
    for (z = 9223372036854775808u; w < 64; z >>= 1, ++w)
    {
        b[w] = ((x & z) == z) ? '1' : '0';
    }

    return b;
}

/*
 * Test shall be run on little-endian machine (x86)
 */

TEST(RFProtocolTest, HammingWeight) {
  EXPECT_EQ(0, hammingw(0));
  EXPECT_EQ(1, hammingw(1));
  EXPECT_EQ(1, hammingw(2));
  EXPECT_EQ(2, hammingw(3));
  EXPECT_EQ(3, hammingw(42));
  EXPECT_EQ(5, hammingw(666));
  EXPECT_EQ(33, hammingw(5555555555555555555));
}

TEST(RFProtocolTest, CreatePacket) {
  uint8_t message[1] = { 170 };
  packet_t packet;
  uint8_t *rest = message;
  create_packet(12, 248, 1, message, &packet, &rest);
  EXPECT_EQ(0xAA010000007C06B2, packet);
  EXPECT_EQ(NULL, rest);
}

TEST(RFProtocolTest, CreatePacketLongMessage) {
  uint8_t message[3] = { 170, 42, 254 };
  packet_t packet;
  uint8_t *rest = message;
  create_packet(12, 248, 3, message, &packet, &rest);
  EXPECT_EQ(0xAA030000007C86B2, packet);
  EXPECT_EQ(&message[1], rest);
}

TEST(RFProtocolTest, ReadPacket) {
  packet_t ipacket;
  // magic = MAGIC_NUMBER;
  // stype = 52; // 3 bits
  // sid = 196; // 3 bits
  // mlength = 5; // 2 bit
  // message = 29; // 4 bits
  // parity = 0; // 3 + 3 + 1 + 3 = 10 -> even number of 1
  ipacket = 0x1D05000000621AB2;
  packet_s opacket;
  bzero(&opacket, sizeof(packet_s));
  int ret = read_packet(ipacket, &opacket);
  EXPECT_EQ(0, ret);
  EXPECT_EQ(52,           opacket.stype);
  EXPECT_EQ(196,          opacket.sid);
  EXPECT_EQ(5,            opacket.mlength);
  EXPECT_EQ(29,           opacket.message);
}

TEST(RFProtocolTest, ReadPacketWrongMagicNumber) {
  packet_t ipacket;
  // magic = 42;
  // stype = 52; // 3 bits
  // sid = 196; // 3 bits
  // mlength = 1; // 1 bit
  // message = 28; // 3 bits
  // parity = 0; // 3 + 3 + 1 + 3 = 10 -> even number of 1
  ipacket = 0x1C01000000621A2A;
  packet_s opacket;
  bzero(&opacket, sizeof(packet_s));
  int ret = read_packet(ipacket, &opacket);
  EXPECT_EQ(WRONG_MAGIC_ERROR, ret);
}

TEST(RFProtocolTest, ReadPacketParityError) {
  packet_t ipacket;
  // magic = MAGIC_NUMBER;
  // stype = 52; // 3 bits
  // sid = 196; // 3 bits
  // mlength = 1; // 1 bit
  // message = 28; // 3 bits
  // parity = 0; // 3 + 3 + 1 + 3 = 10 -> even number of 1
  //   set to 1 for this test
  ipacket = 0x1C01000000629AB2;
  packet_s opacket;
  bzero(&opacket, sizeof(packet_s));
  int ret = read_packet(ipacket, &opacket);
  EXPECT_EQ(PARITY_ERROR, ret);
}

TEST(RFProtocolTest, ReadMessage) {
  uint8_t *buffer = new uint8_t[sizeof(uint8_t) * 9];
  bzero(buffer, sizeof(uint8_t) * 17);
  packet_t ipacket;
  // magic = MAGIC_NUMBER;
  // stype = 52; // 3 bits
  // sid = 196; // 3 bits
  // mlength = 5; // 1 bit
  // message = 28; // 3 bits
  // parity = 0; // 3 + 3 + 1 + 3 = 11 -> even number of 1
  ipacket = 0x1C05000000621AB2;
  // Copy the header into the buffer and append an additional ictet
  memcpy(buffer, &ipacket, sizeof(packet_t));
  buffer[8] = 69;

  // Observed outcome
  packet_s opacket;
  bzero(&opacket, sizeof(packet_s));
  uint8_t *messagerest;
  int ret = read_message(buffer, &opacket, &messagerest);
  EXPECT_EQ(0,    ret);
  EXPECT_EQ(52,   opacket.stype);
  EXPECT_EQ(196,  opacket.sid);
  EXPECT_EQ(5,    opacket.mlength);
  EXPECT_EQ(28,   opacket.message);
  EXPECT_EQ(&buffer[8], messagerest);
  delete[] buffer;
}

TEST(RFProtocolTest, MagSensor) {
  packet_t ipacket;
  uint8_t message = 0;
  create_packet(5, 1, 1, &message, &ipacket, NULL);
  packet_s opacket;
  read_packet(ipacket, &opacket);
  EXPECT_EQ(5, opacket.stype);
  EXPECT_EQ(1, opacket.sid);
  EXPECT_EQ(1, opacket.mlength);
  EXPECT_EQ(0, opacket.message);
}
