#include <ingame_state_recv_packet_factory.hpp>
#include <binary_deserializer.hpp>
#include <null_received_packet.hpp>
#include <c2s/join_chat.hpp>
#include <c2s/leave_chat.hpp>
#include <c2s/log_off_to_lobby.hpp>
#include <c2s/send_chat_msg.hpp>
#include <c2s/friend_add.hpp>
#include <c2s/friend_remove.hpp>
#include <ingame_state_protocol.hpp>

ingame_state_recv_packet_factory::ingame_state_recv_packet_factory(state_transitioner& transitioner, user_environment& my_environment)
  : transitioner_(transitioner),
    my_environment_(my_environment)
{
}

received_packet::packet_ptr ingame_state_recv_packet_factory::construct(const std::array<char, 2048>& data, unsigned size)
{
  binary_deserializer bd(data.data(), size);
  ingame_state_protocol::to_server_packet_opcodes opcode;
  bd >> opcode;
  switch (opcode)
  {
    case ingame_state_protocol::join_chat:
      return std::make_unique<join_chat>(bd, my_environment_);
    case ingame_state_protocol::leave_chat:
      return std::make_unique<leave_chat>(bd, my_environment_);
    case ingame_state_protocol::log_off_to_lobby:
      return std::make_unique<log_off_to_lobby>(transitioner_, my_environment_);
    case ingame_state_protocol::send_chat_msg:
      return std::make_unique<send_chat_msg>(bd, my_environment_);
    case ingame_state_protocol::friend_add:
      return std::make_unique<friend_add>(bd, my_environment_);
    case ingame_state_protocol::friend_remove:
      return std::make_unique<friend_remove>(bd, my_environment_);
  }
  return std::make_unique<null_received_packet>();
}
