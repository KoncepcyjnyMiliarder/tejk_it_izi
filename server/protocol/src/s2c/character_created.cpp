#include <s2c/character_created.hpp>
#include <binary_serializer.hpp>
#include <lobby_state_protocol.hpp>

unsigned lobby_packet_constructors::character_created(std::array<char, 2048>& buffer_to_fill, const lobby_character& new_character)
{
  binary_serializer bs(buffer_to_fill);
  bs << lobby_state_protocol::character_created << new_character.uid << new_character.name;
  return bs.get_current_size();
}