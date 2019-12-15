#pragma once

namespace login_protocol
{

enum class to_client_packet_opcodes : char
{
  account_already_online,
  here_have_a_cookie,
  wrong_username,
  wrong_password,
};

enum class to_server_packet_opcodes : char
{
  request_login
};

} // namespace login_protocol

namespace gs_ls_protocol
{

enum class to_ls_packet_opcodes : char
{
  validate_cookie,
  user_no_longer_online
};

enum class to_gs_packet_opcodes : char
{
  kick_user,
  cookie_is_invalid,
  cookie_succesfully_used
};

} // namespace gs_ls_protocol
