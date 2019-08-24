#include <binary_serializer.hpp>
#include <ingame_state_protocol.hpp>
#include <s2c/chat_msg.hpp>

unsigned
ingame_packet_constructors::chat_msg(net_socket::buffer& buffer_to_fill,
                                     const std::string& chatroom,
                                     const std::string& user,
                                     const std::string& msg)
{
  binary_serializer bs(buffer_to_fill);
  bs << ingame_state_protocol::to_client_packet_opcodes::chat_msg << chatroom << user << msg;
  return bs.get_current_size();
}
