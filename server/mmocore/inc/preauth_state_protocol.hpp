#pragma once

namespace preauth_state_protocol
{

enum to_client_packet_opcodes : char
{
  good_boy_good_keys,
  fuck_you_bad_keys,
};

enum to_server_packet_opcodes : char
{
  loginserver_keys,
};

} // namespace preauth_state_protocol