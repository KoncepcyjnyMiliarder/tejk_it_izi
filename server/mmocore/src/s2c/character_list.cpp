#include <binary_serializer.hpp>
#include <lobby_state_protocol.hpp>
#include <s2c/character_list.hpp>

unsigned
lobby_packet_constructors::character_list(std::array<char, 2048>& buffer_to_fill,
                                          const lobby_character::lobby_character_list& lobby_chars)
{
  binary_serializer bs(buffer_to_fill);
  bs << lobby_state_protocol::character_list << lobby_chars.size();
  for (const auto& character : lobby_chars)
    bs << character.uid << character.name;
  return bs.get_current_size();
}
