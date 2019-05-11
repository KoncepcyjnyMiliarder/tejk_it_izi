#include <c2s/log_off_to_lobby.hpp>
#include <lobby_state.hpp>

log_off_to_lobby::log_off_to_lobby(state_transitioner& transitioner, user_environment& my_environment)
  : transitioner_(transitioner),
    my_environment_(my_environment)
{
}

void log_off_to_lobby::execute_associated_action()
{
  transitioner_.transition(std::make_unique<lobby_state>(transitioner_, my_environment_.my_session_,
                           my_environment_.universe_, my_environment_.logger_, my_environment_.db_,
                           my_environment_.acc_data_)); //C A R E F U L
}
