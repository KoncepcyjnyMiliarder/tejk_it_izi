#include <s2c/character_selected.hpp>
#include <binary_serializer.hpp>
#include <lobby_state_protocol.hpp>

unsigned lobby_packet_constructors::character_selected(std::array<char, 2048>& buffer_to_fill, const lobby_character& character)
{
  binary_serializer bs(buffer_to_fill);
  bs << lobby_state_protocol::character_selected << character.name;
  return bs.get_current_size();
}