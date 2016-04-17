#include <iostream>
#include "gtest/gtest.h"
#include "test_protocol.hh"
#include "test_bitop.hh"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
