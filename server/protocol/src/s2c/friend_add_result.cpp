#include <s2c/friend_add_result.hpp>
#include <binary_serializer.hpp>
#include <ingame_state_protocol.hpp>

unsigned ingame_packet_constructors::friend_add_result(net_socket::buffer& buffer_to_fill, friend_add_request_result result, const std::string& name)
{
  binary_serializer bs(buffer_to_fill);
  bs << ingame_state_protocol::to_client_packet_opcodes::friend_add_result << result << name;
  return bs.get_current_size();
}
