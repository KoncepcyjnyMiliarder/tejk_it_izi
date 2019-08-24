#include <binary_serializer.hpp>
#include <ingame_state_protocol.hpp>
#include <s2c/friend_online_while_you_login.hpp>

unsigned
ingame_packet_constructors::friend_online_while_you_login(net_socket::buffer& buffer_to_fill,
                                                          const std::string& name)
{
  binary_serializer bs(buffer_to_fill);
  bs << ingame_state_protocol::to_client_packet_opcodes::friend_online_while_you_login << name;
  return bs.get_current_size();
}
