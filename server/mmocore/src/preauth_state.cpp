#include <preauth_state.hpp>
#include <lobby_state.hpp>
#include <binary_deserializer.hpp>
#include <preauth_state_protocol.hpp>

std::array<char, 2048> preauth_state::ok_response_ = { preauth_state_protocol::good_boy_good_keys };
std::array<char, 2048> preauth_state::fuck_you_response_ = { preauth_state_protocol::fuck_you_bad_keys };

preauth_state::preauth_state(state_transitioner& transitioner, std::shared_ptr<net_session> my_session, world& universe,
                             logger& logger, login_validator& authenticator, database_facade& db)
  : transitioner_(transitioner),
    my_session_(my_session),
    world_(universe),
    logger_(logger),
    authenticator_(authenticator),
    db_(db)
{
}

void preauth_state::handle_network_packet(const std::array<char, 2048>& data, unsigned len)
{
  binary_deserializer bd(data.data(), len);
  preauth_state_protocol::to_server_packet_opcodes opcode;
  bd >> opcode;

  auto session = my_session_; //this is cause the object can commit suicide if it's the enterworld packet. I got to think of solution

  if (opcode == preauth_state_protocol::loginserver_keys)
  {
    std::string loginserver_key;
    std::string account_login;

    try
    {
      bd >> loginserver_key >> account_login;
      if (loginserver_key.size() != 64)
        throw std::underflow_error("loginserver_key is not 64 in size");
      if (authenticator_.are_loginkey_and_username_valid(loginserver_key, account_login))
      {
        my_session_->send_to_client(ok_response_, 1);
        transitioner_.transition(std::make_unique<lobby_state>(transitioner_, my_session_, world_, logger_, db_, db_.get_account_data(account_login)));
        session->do_recv(); // calling do_recv again explicitly, or scheduling database action while socket is not listening
        return; //make sure ; O
      }
      else
      {
        logger_.log("Rip in pepperonis, the loginserver doesnt recognize you: " + account_login);
        my_session_->send_to_client(fuck_you_response_, 1);
        my_session_->force_close();
      }
    }
    catch (const std::underflow_error& e)
    {
      logger_.log(e.what());
      my_session_->force_close();
    }
  }
  else
  {
    logger_.log("Rip in pepperonis, unknown packet opcode: " + std::to_string(opcode));
    my_session_->send_to_client(fuck_you_response_, 1);
    my_session_->force_close();
  }
}

void preauth_state::start()
{
}
