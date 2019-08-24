#include <ingame_state.hpp>

ingame_state::ingame_state(state_transitioner& transitioner,
                           std::shared_ptr<net_session> my_session,
                           world& universe,
                           logger& logger,
                           const lobby_character& selected_char,
                           database_facade& db,
                           account_data acc_data,
                           asynchronous_database_adapter& async_db)
  : transitioner_(transitioner)
  , fin_(my_session)
  , my_environment_(my_session, universe, selected_char, logger, db, acc_data, async_db)
  , recv_factory_(transitioner_, my_environment_)
  , async_db_(async_db)
{
  universe.online_player_registry_.register_me(my_environment_.my_character_);
}

ingame_state::~ingame_state()
{
  my_environment_.universe_.online_player_registry_.unregister_me(my_environment_.my_character_);
  my_environment_.logger_.log_diagnostic(__func__);
}

void
ingame_state::handle_network_packet(const std::array<char, 2048>& data, unsigned len)
{
  try
  {
    auto ptr = recv_factory_.construct(data, len);
    ptr->execute_associated_action();
    if (transitioner_.async_state_ ==
        state_transitioner::async_operation_status::awaiting_packet_poll)
      my_environment_.my_session_->do_recv();
  }
  catch (const std::exception& e)
  {
    my_environment_.logger_.log_diagnostic(e.what());
    my_environment_.my_session_->force_close();
  }
}

void
ingame_state::start()
{
  my_environment_.logger_.log_diagnostic(__func__);
  my_environment_.my_session_->do_recv();
  // send enterworld info, at the moment (chat system only) nothing is actually
  // needed
}
