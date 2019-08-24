#pragma once

#include <array>
#include <lobby_character.hpp>

namespace lobby_packet_constructors
{
unsigned character_selected(std::array<char, 2048>& buffer_to_fill,
                            const lobby_character& character);
}