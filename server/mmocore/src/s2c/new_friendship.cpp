#include <binary_serializer.hpp>
#include <ingame_state_protocol.hpp>
#include <s2c/new_friendship.hpp>

unsigned
ingame_packet_constructors::new_friendship(net_socket::buffer& buffer_to_fill,
                                           const std::string& name)
{
  binary_serializer bs(buffer_to_fill);
  bs << ingame_state_protocol::to_client_packet_opcodes::new_friendship << name;
  return bs.get_current_size();
}
