#pragma once

#include <boost/asio.hpp>
#include <console_logger.hpp>
#include <loginserver_data.hpp>

class loginserver
{

  console_logger logger_;
  boost::asio::ip::tcp::acceptor& client_acceptor_;
  boost::asio::ip::tcp::acceptor& gs_acceptor_;
  boost::asio::ip::tcp::socket client_socket_;
  boost::asio::ip::tcp::socket gs_socket_;

  loginserver_data login_data_;

  void do_accept_client();
  void do_accept_gs();

public:
  loginserver(boost::asio::ip::tcp::acceptor& client_acceptor,
              boost::asio::ip::tcp::acceptor& gs_acceptor);

  void run();
};
