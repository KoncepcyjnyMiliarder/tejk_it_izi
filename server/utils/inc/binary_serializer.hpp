#pragma once

#include <string>
#include <array>

class binary_serializer
{

    const char* const begining_;
    char* current_pos_;

  public:

    //miras - Today at 6:52 PM
    //nie uwazasz, ze 2k bufora to dosyc ubogo ?
    binary_serializer(std::array<char, 2048>& buffer_to_fill)
      : begining_(buffer_to_fill.data()),
        current_pos_(buffer_to_fill.data())
    {
    }

    //pointers qualify for pods too qq
    template<typename T>
    binary_serializer& operator<<(const T* ptr) = delete;

    template<typename T>
    typename std::enable_if<std::is_pod<T>::value, binary_serializer&>::type operator << (const T& in);

    inline binary_serializer& operator<<(const std::string& text);

    unsigned get_current_size() const
    {
      return current_pos_ - begining_;
    }
};

template<typename T>
typename std::enable_if<std::is_pod<T>::value, binary_serializer&>::type
binary_serializer::operator << (const T& in)
{
  T* buffer_ptr = reinterpret_cast<T*>(current_pos_);
  *buffer_ptr = in;
  current_pos_ += sizeof(T);
  return *this;
}

binary_serializer& binary_serializer::operator<<(const std::string& text)
{
  unsigned size = text.size();
  *this << size;
  std::copy(text.begin(), text.end(), current_pos_);
  current_pos_ += size;
  return *this;
}