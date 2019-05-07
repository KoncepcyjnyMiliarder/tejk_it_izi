#include <ingame_state.hpp>

ingame_state::ingame_state(state_transitioner& transitioner, std::shared_ptr<net_session> my_session, world& universe,
                           logger& logger, std::string selected_character_name, database_facade& db, account_data acc_data)
  : transitioner_(transitioner),
    my_session_(my_session),
    fin_(*this),
    world_(universe),
    logger_(logger),
    db_(db),
    my_pawn_(world_, my_session, selected_character_name),
    acc_data_(std::move(acc_data)),
    recv_factory_(my_session, transitioner, universe, logger_, db, my_pawn_, acc_data_, chatrooms_im_in_)
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
    logger_.log(e.what());
    my_session_->force_close();
  }
}

void ingame_state::start()
{
  //send enterworld info, at the moment (chat system only) nothing is actually needed
}
