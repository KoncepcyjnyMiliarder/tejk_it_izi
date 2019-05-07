#include <s2c/user_joined_chat.hpp>
#include <binary_serializer.hpp>
#include <ingame_state_protocol.hpp>

unsigned ingame_packet_constructors::user_joined_chat(net_socket::buffer& buffer_to_fill, const std::string& chatroom, const std::string& user)
{
  binary_serializer bs(buffer_to_fill);
  bs << ingame_state_protocol::to_client_packet_opcodes::user_joined_chat << chatroom << user;
  return bs.get_current_size();
}
