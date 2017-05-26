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

TEST(RFProtocolTest, ReadPacketWrongMagicNumber) {
  packet_t ipacket;
  // Magic number is supposed to be 178 (B2)
  ipacket = 0x000000A2;
  packet_s opacket;
  bzero(&opacket, sizeof(packet_s));
  int ret = read_packet(ipacket, &opacket);
  EXPECT_EQ(WRONG_MAGIC_ERROR, ret);
}

TEST(RFProtocolTest, ReadPacketParityError) {
  packet_t ipacket;
  // magic = MAGIC_NUMBER;
  // stype = 3; // 2 bits
  // sid = 8; // 1 bit
  // battery_indicator = 0; // 0 bit
  // message = 78; // 4 bits
  create_packet(3, 8, false, 78, &ipacket);
  // parity = 0; // 2 + 1 + 4 = 7 -> odd number of 1 so paity should be 0
  //   set to 1 for this test
  ipacket |= 1 << 8;
  packet_s opacket;
  bzero(&opacket, sizeof(packet_s));
  int ret = read_packet(ipacket, &opacket);
  EXPECT_EQ(PARITY_ERROR, ret);
}

TEST(RFProtocolTest, MagSensor) {
  packet_t ipacket;
  create_packet(5, 1, false, 0, &ipacket);
  packet_s opacket;
  read_packet(ipacket, &opacket);
  EXPECT_EQ(5,     opacket.stype);
  EXPECT_EQ(false, opacket.battery_indicator);
  EXPECT_EQ(1,     opacket.sid);
  EXPECT_EQ(0,     opacket.message);
}
