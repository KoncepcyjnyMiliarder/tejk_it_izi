#include <ingame_state.hpp>

ingame_state::ingame_state(state_transitioner& transitioner, std::shared_ptr<net_session> my_session, world& universe,
                           logger& logger, const lobby_character& selected_char, database_facade& db, account_data acc_data)
  : transitioner_(transitioner),
    fin_(my_session),
    my_environment_(my_session, universe, selected_char, logger, db, acc_data),
    recv_factory_(transitioner_, my_environment_)
{
}

ingame_state::~ingame_state()
{
}

void ingame_state::handle_network_packet(const std::array<char, 2048>& data, unsigned len)
{
  try
  {
    auto ptr = recv_factory_.construct(data, len);
    ptr->execute_associated_action();
  }
  catch (const std::exception& e)
  {
    my_environment_.logger_.log(e.what());
    my_environment_.my_session_->force_close();
  }
}

void ingame_state::start()
{
  //send enterworld info, at the moment (chat system only) nothing is actually needed
}
