#include <binary_deserializer.hpp>
#include <binary_serializer.hpp>
#include <client_session.hpp>
#include <login_protocol.hpp>

void
client_session::do_recv_header()
{
  async_read(
    sock_,
    boost::asio::buffer(recvbuf_, 4),
    [this, self = shared_from_this()](const boost::system::error_code& ec, std::size_t bytes_read) {
      unsigned len = *(unsigned*)recvbuf_.data();
      if (ec)
        logger_.log_diagnostic(ec.message());
      else if (bytes_read != 4)
        logger_.log_diagnostic(__func__ + std::string(" couldnt read 4 bytes header"));
      else if (len > recvbuf_.size())
        logger_.log_diagnostic(__func__ +
                               std::string(" client expects to send too much bytes of data: ") +
                               std::to_string(len));
      else
        do_recv_data(len);
    });
}

void
client_session::do_recv_data(unsigned expected_len)
{
  async_read(sock_,
             boost::asio::buffer(recvbuf_, expected_len),
             [this, self = shared_from_this(), expected_len](const boost::system::error_code& ec,
                                                             std::size_t bytes_read) {
               if (ec)
                 logger_.log_diagnostic(ec.message());
               else if (bytes_read != expected_len)
                 logger_.log_diagnostic(__func__ + std::string(" couldnt read enough bytes"));
               else
                 handle_packet(expected_len);
             });
}

void
client_session::write(const std::array<char, 2048>& data, unsigned size)
{
  boost::system::error_code ec; // TODO, what on error?
  boost::asio::write(sock_, boost::asio::buffer((const char*)&size, sizeof(unsigned)), ec);
  boost::asio::write(sock_, boost::asio::buffer(data.data(), size), ec);
}

void
client_session::handle_packet(unsigned len)
{
  binary_deserializer bd(recvbuf_.data(), len);
  try
  {
    login_protocol::to_server_packet_opcodes opcode;
    bd >> opcode;
    if (opcode != login_protocol::to_server_packet_opcodes::request_login)
    {
      logger_.log_diagnostic("Received proper packet but opcode if wrong");
      return;
    }
    std::string login;
    std::string pass;
    bd >> login >> pass;
    std::array<char, 2048> reply;
    binary_serializer bs(reply);
    // confirm login/pass
    // TEMP!!
    if (login != "foo" && login != "mirek" && login != "krzysztof")
    {
      bs << login_protocol::to_client_packet_opcodes::wrong_username;
      write(reply, bs.get_current_size());
      return;
    }
    if (pass != "passqq")
    {
      bs << login_protocol::to_client_packet_opcodes::wrong_password;
      write(reply, bs.get_current_size());
      return;
    }
    // if the user is on GS, kick him and await confirmation
    if (login_data_.user_to_gameserver_.count(login))
    {
      auto gs_session =
        login_data_.user_to_gameserver_.at(login)->associated_gameserver_session.lock();
      bs << login_protocol::to_client_packet_opcodes::account_already_online;
      write(reply, bs.get_current_size());
      if (gs_session != nullptr)
        gs_session->request_kick_player(login);
      return;
    }
    // if has valid cookie already, remove old cookie and generate new
    std::string new_cookie = login + "SECRET_COOKIE_xD";
    // this also invalidates previous cookie
    login_data_.validated_username_to_cookie_[login] = new_cookie;
    bs << login_protocol::to_client_packet_opcodes::here_have_a_cookie << new_cookie;
    write(reply, bs.get_current_size());
  }
  catch (const std::exception& e)
  {
    logger_.log_diagnostic(e.what());
  }
  // no need to recursively call recv cause atm it's only 1 packet exchange
  // do_recv_header();
}

client_session::~client_session()
{
  logger_.log_diagnostic(__func__);
}

void
client_session::start()
{
  logger_.log_diagnostic(__func__);
  do_recv_header();
}
