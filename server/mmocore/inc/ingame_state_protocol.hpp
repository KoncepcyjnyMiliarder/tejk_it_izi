#pragma once

namespace ingame_state_protocol
{

enum to_client_packet_opcodes : char
{
  //log off from game
  moved_to_lobby,
  //chat_system
  user_joined_chat,
  user_left_chat,
  chat_msg,
  //friend_system
  friend_online_while_you_login,
  friend_login,
  friend_leave,
  new_friendship,
  broken_friendship,
  friendship_request,
  friend_add_fail,
  friend_removal_fail,
  friend_add_result
};

enum to_server_packet_opcodes : char
{
  //chat_system
  log_off_to_lobby,
  join_chat,
  leave_chat,
  send_chat_msg,
  //friend_system
  friend_add,
  friend_remove
};

}