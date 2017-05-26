#include <strings.h>
#include <math.h>
#include "gtest/gtest.h"
#include "bitop.h"

TEST(BitOpTest, PackTestEmptyBuffer) {
  uint8_t buffer[4];
  bzero(&buffer, sizeof (char) * 4);
  // 682: .... ....  .... ....  .... ..10  1010 1010
  uint32_t value = 682;
  uint8_t size = 10;
  uint32_t new_offset = 0;
  new_offset = pack(value, size, buffer, 0);
  EXPECT_EQ(10, new_offset);
  // expected outcome:
  // buffer: 1010 1010  1000 0000  0000 0000  0000 0000
  EXPECT_EQ(170, buffer[0]);
  EXPECT_EQ(128, buffer[1]);
  EXPECT_EQ(0,   buffer[2]);
  EXPECT_EQ(0,   buffer[3]);
}

TEST(BitOpTest, PackTest) {
  uint8_t buffer[4];
  bzero(&buffer, sizeof (char) * 4);
  // buffer: 1111 1111  111. ....  .... ....  .... ....
  buffer[0] = 255;
  buffer[1] = 224;
  // 682:    .... ....  .... ....  .... ..10  1010 1010
  uint32_t value = 682;
  uint8_t size = 10;
  uint32_t new_offset = 0;
  new_offset = pack(value, size, buffer, 11);
  EXPECT_EQ(21, new_offset);
  // expected outcome:
  // buffer: 1111 1111  1111 0101  0101 0000  0000 0000
  EXPECT_EQ(255, buffer[0]);
  EXPECT_EQ(245, buffer[1]);
  EXPECT_EQ(80,  buffer[2]);
  EXPECT_EQ(0,   buffer[3]);
}

TEST(BitOpTest, PackTestOnBitAtATime) {
  uint8_t buffer[4];
  bzero(&buffer, sizeof (char) * 4);
  uint32_t new_offset = 0;
  new_offset = pack(1, 1, buffer, new_offset);
  new_offset = pack(1, 1, buffer, new_offset);
  new_offset = pack(1, 1, buffer, new_offset);
  new_offset = pack(1, 1, buffer, new_offset);

  new_offset = pack(1, 1, buffer, new_offset);
  new_offset = pack(1, 1, buffer, new_offset);
  new_offset = pack(1, 1, buffer, new_offset);
  EXPECT_EQ(7,   new_offset);
  EXPECT_EQ(254, buffer[0]);
  EXPECT_EQ(0,   buffer[1]);
  EXPECT_EQ(0,   buffer[2]);
  EXPECT_EQ(0,   buffer[3]);
}

TEST(BitOpTest, UnpackTest) {
  uint8_t buffer[4];
  bzero(&buffer, sizeof (char) * 4);
  buffer[2] = 0x1E;
  uint64_t value = 0;
  unpack(&value, 4, buffer, 19);
  EXPECT_EQ(0xF, value);
}

TEST(BitOpTest, UnpackTestComplex) {
  uint8_t buffer[4];
  bzero(&buffer, sizeof (char) * 4);
  buffer[1] = 0x02;
  buffer[2] = 0xB2;
  uint64_t value = 0;
  unpack(&value, 9, buffer, 14);
  EXPECT_EQ(0x159, value);
}

TEST(BitOpTest, UnpackTestComplex2) {
  uint8_t buffer[8];
  bzero(&buffer, sizeof (char) * 8);
  buffer[0] = 0xB2;
  buffer[1] = 0x1A;
  buffer[2] = 0x62;
  buffer[3] = 0x00;
  buffer[4] = 0x00;
  buffer[5] = 0x00;
  buffer[6] = 0x01;
  buffer[7] = 0x31;
  uint64_t value = 0;
  unpack(&value, 8, buffer, 9);
  EXPECT_EQ(52, value);
}

TEST(BitOpTest, PackUnpack) {
  uint8_t buffer[4];
  for (uint8_t offset = 0; offset < 32; ++offset) {
    for (uint8_t size = 1; size <= 32 - offset; ++size) {
      bzero(&buffer, sizeof (char) * 4);
      pack(pow(2, size) - 1, size, buffer, offset);
      uint32_t value = 0;
      unpacki(&value, size, buffer, offset);
      EXPECT_EQ(pow(2, size) - 1, value);
    }
  }
}

