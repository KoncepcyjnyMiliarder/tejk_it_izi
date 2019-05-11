#include <s2c/friend_removal_fail.hpp>
#include <binary_serializer.hpp>
#include <ingame_state_protocol.hpp>

unsigned ingame_packet_constructors::friend_removal_fail(net_socket::buffer& buffer_to_fill, const std::string& name)
{
  binary_serializer bs(buffer_to_fill);
  bs << ingame_state_protocol::to_client_packet_opcodes::friend_removal_fail << name;
  return bs.get_current_size();
}
