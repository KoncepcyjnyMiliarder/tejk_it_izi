#include <c2s/request_lobby_log_off.hpp>

request_lobby_log_off::request_lobby_log_off(std::shared_ptr<net_session> my_session)
  : my_session_(my_session)
{
}

void
request_lobby_log_off::execute_associated_action()
{
  my_session_->force_close();
}
