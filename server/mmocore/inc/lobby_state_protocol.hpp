#pragma once

namespace lobby_state_protocol
{

enum to_client_packet_opcodes : char
{
  character_list,
  character_selected,
  character_created,
  character_creation_fail,
};

enum to_server_packet_opcodes : char
{
  select_character,
  create_character,
  delete_character,
  log_off,
};

} // namespace lobby_state_protocol
