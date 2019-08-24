#include <binary_serializer.hpp>
#include <ingame_state_protocol.hpp>
#include <s2c/moved_to_lobby.hpp>

unsigned
ingame_packet_constructors::moved_to_lobby(net_socket::buffer& buffer_to_fill)
{
  binary_serializer bs(buffer_to_fill);
  bs << ingame_state_protocol::to_client_packet_opcodes::moved_to_lobby;
  return bs.get_current_size();
}
