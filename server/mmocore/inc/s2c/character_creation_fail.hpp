#pragma once

#include <array>

enum class character_creation_fail_reason
{
  name_too_long,
  name_contains_incorrect_digits,
  name_already_used,
  character_limit_exceeded,
};

namespace lobby_packet_constructors
{
unsigned character_creation_fail(std::array<char, 2048>& buffer_to_fill,
                                 character_creation_fail_reason reason);
}