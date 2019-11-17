#include <binary_deserializer.hpp>
#include <binary_serializer.hpp>
#include <gameserver_session.hpp>
#include <login_protocol.hpp>

void
gameserver_session::do_recv_header()
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
gameserver_session::do_recv_data(unsigned expected_len)
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
gameserver_session::request_kick_player(const std::string& name)
{
	std::array<char, 2048> reply;
	binary_serializer bs(reply);
	bs << gs_ls_protocol::to_gs_packet_opcodes::kick_user << name;
	write(reply, bs.get_current_size());
}

void
gameserver_session::write(const std::array<char, 2048>& data, unsigned size)
{
  boost::system::error_code ec; // TODO, what on error?
  boost::asio::write(sock_, boost::asio::buffer((const char*)&size, sizeof(unsigned)), ec);
  boost::asio::write(sock_, boost::asio::buffer(data.data(), size), ec);
}

void
gameserver_session::handle_packet(unsigned len)
{
  binary_deserializer bd(recvbuf_.data(), len);
  try
  {
    gs_ls_protocol::to_ls_packet_opcodes opcode;
    bd >> opcode;
    switch (opcode)
    {
      case gs_ls_protocol::to_ls_packet_opcodes::validate_cookie:
      {
        std::string login, cookie;
        bd >> login >> cookie;
        if (login_data_.validated_username_to_cookie_.count(login) == 0 ||
            login_data_.validated_username_to_cookie_.at(login) != cookie)
        {
          logger_.log_diagnostic("User did not validate: " + login);
          std::array<char, 2048> reply;
          binary_serializer bs(reply);
          bs << gs_ls_protocol::to_gs_packet_opcodes::cookie_is_invalid << login;
          write(reply, bs.get_current_size());
        }
        else
        {
          logger_.log_diagnostic("Succesfully logging in user: " + login);
          login_data_.validated_username_to_cookie_.erase(login);
          std::array<char, 2048> reply;
          binary_serializer bs(reply);
          bs << gs_ls_protocol::to_gs_packet_opcodes::cookie_succesfully_used << login;
          write(reply, bs.get_current_size());
        }
        break;
      }
      case gs_ls_protocol::to_ls_packet_opcodes::user_no_longer_online:
      {
        std::string login;
        bd >> login;
        if (login_data_.user_to_gameserver_.count(login) == 0)
          logger_.log_diagnostic("Received logoff notification for user that wasn't logged in: " +
                                 login);
        else if (login_data_.user_to_gameserver_.at(login)->associated_gameserver_session.lock() !=
                 shared_from_this())
          logger_.log_diagnostic(
            "Received logoff notification from different gameserver than user is online on: " +
            login);
        else
        {
          logger_.log_diagnostic("Loggin off: " + login);
          login_data_.user_to_gameserver_.erase(login);
        }
        break;
      }
      default:
        logger_.log_diagnostic("Received proper packet but opcode if wrong");
        break;
    }
  }
  catch (const std::exception& e)
  {
    logger_.log_diagnostic(e.what());
  }
  do_recv_header();
}

gameserver_session::gameserver_session(boost::asio::ip::tcp::socket&& sock,
                                       logger& logger,
                                       loginserver_data& login_data)
  : sock_(std::move(sock))
  , logger_(logger)
  , login_data_(login_data)
{
  logger_.log_diagnostic(__func__);
}

gameserver_session::~gameserver_session()
{
  logger_.log_diagnostic(__func__);
  login_data_.gses_.erase(associated_gs_);
}

void
gameserver_session::start()
{
  logger_.log_diagnostic(__func__);
  associated_gs_ = std::make_shared<gameserver>();
  associated_gs_->associated_gameserver_session = shared_from_this();
  login_data_.gses_.insert(associated_gs_);
}
