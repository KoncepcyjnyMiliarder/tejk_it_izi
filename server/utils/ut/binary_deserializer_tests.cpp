#include <gtest/gtest.h>
#include <binary_deserializer.hpp>

TEST(binary_deserializer, initial_test)
{
  std::string data("asdasd");
  binary_deserializer bd(data);
}

TEST(binary_deserializer, read_int)
{
  std::string data("\x11\x22\x33\x44", 4);
  binary_deserializer bd(data);
  int val;
  bd >> val;
  EXPECT_EQ(0x44332211, val);
}

TEST(binary_deserializer, read_char)
{
  std::string data("\x11\x22\x33\x44", 4);
  binary_deserializer bd(data);
  char val;
  bd >> val;
  EXPECT_EQ(0x11, val);
}

TEST(binary_deserializer, throw_on_underflow)
{
  std::string data("\x11", 1);
  binary_deserializer bd(data);
  short val;
  EXPECT_THROW(bd >> val, std::underflow_error);
}

TEST(binary_deserializer, throw_when_past_end)
{
  std::string data("\x11\x22\x33\x44\x11\x22\x33", 7);
  binary_deserializer bd(data);
  int val;
  EXPECT_NO_THROW(bd >> val);
  char val2;
  EXPECT_NO_THROW(bd >> val2);
  EXPECT_THROW(bd >> val, std::underflow_error);
}

TEST(binary_deserializer, serialize_pod_struct)
{
  std::string data("\x11\x22\x33\x44\x11\x22\x33\x44", 8);
  binary_deserializer bd(data);
  struct some_pod
  {
    int a;
    short c;
    char b;
  };
  some_pod expected{ 0x44332211, 0x2211, 0x33 };
  some_pod actual;
  bd >> actual;
  EXPECT_EQ(expected.a, actual.a);
  EXPECT_EQ(expected.b, actual.b);
  EXPECT_EQ(expected.c, actual.c);
}

TEST(binary_deserializer, serialzie_string)
{
  std::string data("\x08\x00\x00\x00""dupadupa", 12);
  binary_deserializer bd(data);
  std::string text;
  EXPECT_NO_THROW(bd >> text);
  EXPECT_EQ(text.size(), 8);
  EXPECT_EQ(text, std::string("dupadupa"));
}

TEST(binary_deserializer, empty_string)
{
  std::string data("\x00\x00\x00\x00", 4);
  binary_deserializer bd(data);
  std::string text;
  EXPECT_NO_THROW(bd >> text);
  EXPECT_EQ(text.size(), 0);
  EXPECT_EQ(text, std::string());
}

TEST(binary_deserializer, serialzie_two_strings)
{
  std::string data("\x08\x00\x00\x00""dupadupa""\x0A\x00\x00\x00""something33", 26);
  binary_deserializer bd(data);
  std::string dupadupa, something;
  EXPECT_NO_THROW(bd >> dupadupa);
  EXPECT_NO_THROW(bd >> something);
  EXPECT_EQ(dupadupa, std::string("dupadupa"));
  EXPECT_EQ(something, std::string("something33"));
}

TEST(binary_deserializer, serialzie_text_with_pods)
{
  std::string data("\x08\x00\x00\x00""dupadupa""\x01\x02\x03\x04\x05""\x0A\x00\x00\x00""something33USELESS_DATA", 43);
  binary_deserializer bd(data);
  std::string dupadupa, something;
  int a;
  unsigned char b;
  EXPECT_NO_THROW(bd >> dupadupa);
  EXPECT_NO_THROW(bd >> a);
  EXPECT_NO_THROW(bd >> b);
  EXPECT_NO_THROW(bd >> something);
  EXPECT_EQ(dupadupa, std::string("dupadupa"));
  EXPECT_EQ(a, 0x04030201);
  EXPECT_EQ(b, 0x5);
  EXPECT_EQ(something, std::string("something33"));
}

TEST(binary_deserializer, manipulator_skip)
{
  std::string data("\x11", 1);
  binary_deserializer bd(data);
  char val;
  EXPECT_NO_THROW(bd >> val >> serializer_skip(7));
}

TEST(binary_deserializer, serialize_with_skip)
{
  std::string data("\x11\x22\x33\x44\x11\x22\x33\x44", 8);
  binary_deserializer bd(data);
  char val;
  int val2;
  bd >> val >> serializer_skip(2) >> val2;
  EXPECT_EQ(0x11, val);
  EXPECT_EQ(0x33221144, val2);
}

TEST(binary_deserializer, custom_struct_manipulator)
{
  std::string data("\x11\x22\x33\x44\x11\x22\x33\x44", 8);
  binary_deserializer bd(data);

  struct test_manipulator_rewind
  {
    void operator()(const std::string& data, std::size_t& current_pos)
    {
      current_pos = 0u;
    }
  };

  char c1, c2;
  int val2;
  bd >> c1 >> c2 >> test_manipulator_rewind() >> val2;
  EXPECT_EQ(0x11, c1);
  EXPECT_EQ(0x22, c2);
  EXPECT_EQ(0x44332211, val2);
}

namespace
{
void test_function(const std::string& data, std::size_t& current_pos)
{
  current_pos = 0u;
}
}

TEST(binary_deserializer, custom_function_manipulator)
{
  std::string data("\x11\x22\x33\x44\x11\x22\x33\x44", 8);
  binary_deserializer bd(data);

  char c1, c2;
  int val2;
  bd >> c1 >> c2 >> test_function >> val2;
  EXPECT_EQ(0x11, c1);
  EXPECT_EQ(0x22, c2);
  EXPECT_EQ(0x44332211, val2);
}

TEST(binary_deserializer, custom_lambda_manipulator)
{
  std::string data("\x11\x22\x33\x44\x11\x22\x33\x44", 8);
  binary_deserializer bd(data);

  char c1, c2;
  int val2;
  bd >> c1 >> c2 >>
     [](const std::string & data, std::size_t& current_pos)
  {
    current_pos = 0u;
  }
      >> val2;
  EXPECT_EQ(0x11, c1);
  EXPECT_EQ(0x22, c2);
  EXPECT_EQ(0x44332211, val2);
}