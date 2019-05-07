#pragma once

namespace ingame_state_protocol
{

enum to_client_packet_opcodes : char
{
  moved_to_lobby,
  user_joined_chat,
  user_left_chat,
  chat_msg,
};

enum to_server_packet_opcodes : char
{
  log_off_to_lobby,
  join_chat,
  leave_chat,
  send_chat_msg,
};

}