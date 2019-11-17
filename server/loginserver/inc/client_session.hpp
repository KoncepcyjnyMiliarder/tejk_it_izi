#pragma once

#include <boost/asio.hpp>
#include <logger.hpp>
#include <loginserver_data.hpp>

class client_session : public std::enable_shared_from_this<client_session>
{
  boost::asio::ip::tcp::socket sock_;
  logger& logger_;
  loginserver_data& login_data_;

  std::array<char, 2048> recvbuf_;

  void do_recv_header();
  void do_recv_data(unsigned expected_len);
  void write(const std::array<char, 2048>& data, unsigned size);

  void handle_packet(unsigned len);

public:
  client_session(boost::asio::ip::tcp::socket&& sock, logger& logger, loginserver_data& login_data)
    : sock_(std::move(sock))
    , logger_(logger)
    , login_data_(login_data)
  {
  }
  ~client_session();

  void start();
};
