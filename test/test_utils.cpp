#include "utils.h"
#include <string>
#include <gtest/gtest.h>

TEST(TChat, EncodeMessage) {
    std::string msg = encode_message(12, 4123, "hello bro!");
    EXPECT_EQ(msg, "12.4123.hello bro!");
} // EncodeMessage

TEST(TChat, DecodeMessage) {
    int src, dest;
    std::string msg = decode_message("12.4123.hello bro!", src, dest);
    EXPECT_EQ(src, 12);
    EXPECT_EQ(dest, 4123);
    EXPECT_EQ(msg, "hello bro!");
} // DecodeMessage

