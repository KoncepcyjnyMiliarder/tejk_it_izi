#include <lobby_state.hpp>
#include <s2c/character_list.hpp>

lobby_state::lobby_state(state_transitioner& transitioner, std::shared_ptr<net_session> my_session, world& universe, logger& logger, database_facade& db, account_data acc_data)
  : transitioner_(transitioner),
    my_session_(my_session),
    world_(universe),
    logger_(logger),
    db_(db),
    acc_data_(acc_data),
    recv_factory_(my_session, transitioner, world_, logger_, lobby_chars_, db, acc_data_)
{
  logger_.log_diagnostic(__func__);
}

void lobby_state::handle_network_packet(const std::array<char, 2048>& data, unsigned len)
{
  try
  {
    auto session = my_session_; //this is cause the object can commit suicide if it's the enterworld packet. I got to think of solution
    auto ptr = recv_factory_.construct(data, len);
    ptr->execute_associated_action();
    if(transitioner_.async_state_ == state_transitioner::async_operation_status::awaiting_packet_poll)
      session->do_recv(); // calling do_recv again explicitly, or scheduling database action while socket is not listening
  }
  catch (const std::exception& e)
  {
    logger_.log_diagnostic(e.what());
    my_session_->force_close();
  }
}

void lobby_state::start()
{
  logger_.log_diagnostic(__func__);
  my_session_->do_recv();
  lobby_chars_ = db_.get_lobby_chars(acc_data_.uid);
  std::array<char, 2048> buffer;
  auto len = lobby_packet_constructors::character_list(buffer, lobby_chars_);
  my_session_->send_to_client(buffer, len);
}
