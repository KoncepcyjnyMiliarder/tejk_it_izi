#include <gtest/gtest.h>
#include <binary_serializer.hpp>

TEST(binary_serializer, initial_test)
{
  std::array<char, 2048> buffer;
  binary_serializer bs(buffer);
}

TEST(binary_serializer, serialize_int)
{
  std::pair<int, std::string> tests[] =
  {
    { 0, { "\x00\x00\x00\x00", 4 } },
    { 1, { "\x01\x00\x00\x00", 4 } },
    { -2, { "\xFE\xFF\xFF\xFF", 4 } },
    { 0xDEAD, { "\xAD\xDE\x00\x00", 4 } },
    { 123456789, { "\x15\xCD\x5B\x07", 4 } }, //75BCD15
    { -567890, { "\xAE\x55\xF7\xFF", 4 } }, // FFF755AE
  };

  for (const auto& test : tests)
  {
    std::array<char, 2048> buffer;
    binary_serializer bs(buffer);
    bs << test.first;
    EXPECT_EQ(bs.get_current_size(), test.second.size());
    std::string temp(&buffer[0], test.second.size());
    EXPECT_EQ(temp, test.second);
  }
}

TEST(binary_serializer, serialize_pod_struct)
{
  struct
  {
    int a, b, c;
    char d, e, f, g;
    unsigned x, y, z;
  } test { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  std::array<char, 2048> buffer;
  binary_serializer bs(buffer);
  bs << test;
  std::string expected_str("\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04\x05\x06\x07"
                           "\x08\x00\x00\x00\x09\x00\x00\x00\x0A\x00\x00\x00", sizeof(test));
  EXPECT_EQ(bs.get_current_size(), expected_str.size());
  std::string temp(&buffer[0], expected_str.size());
  EXPECT_EQ(temp, expected_str);
}

TEST(binary_serializer, text_serialization)
{
  std::array<char, 2048> buffer;
  binary_serializer bs(buffer);
  bs << std::string("asd") << std::string("qq") << std::string("lul");
  std::string expected_str("\x03\x00\x00\x00""asd""\x02\x00\x00\x00""qq""\x03\x00\x00\x00""lul", 20);
  EXPECT_EQ(bs.get_current_size(), expected_str.size());
  std::string temp(&buffer[0], expected_str.size());
  EXPECT_EQ(temp, expected_str);
}

TEST(binary_serializer, empty_text)
{
  std::array<char, 2048> buffer;
  binary_serializer bs(buffer);
  bs << std::string();
  std::string expected_str("\x00\x00\x00\x00", 4);
  EXPECT_EQ(bs.get_current_size(), expected_str.size());
  std::string temp(&buffer[0], expected_str.size());
  EXPECT_EQ(temp, expected_str);
}

TEST(binary_serializer, text_with_pods_at_once)
{
  std::array<char, 2048> buffer;
  binary_serializer bs(buffer);
  int a = -1;
  unsigned b = 0xDEADBEEF;
  bs << std::string("misio") << a << b;
  std::string expected_str("\x05\x00\x00\x00""misio\xFF\xFF\xFF\xFF\xEF\xBE\xAD\xDE", 17);
  EXPECT_EQ(bs.get_current_size(), expected_str.size());
  std::string temp(&buffer[0], expected_str.size());
  EXPECT_EQ(temp, expected_str);
}