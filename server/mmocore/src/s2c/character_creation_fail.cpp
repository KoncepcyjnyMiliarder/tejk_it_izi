#include <s2c/character_creation_fail.hpp>
#include <binary_serializer.hpp>
#include <lobby_state_protocol.hpp>

unsigned lobby_packet_constructors::character_creation_fail(std::array<char, 2048>& buffer_to_fill, character_creation_fail_reason reason)
{
  binary_serializer bs(buffer_to_fill);
  bs << lobby_state_protocol::character_creation_fail << reason;
  return bs.get_current_size();
}
