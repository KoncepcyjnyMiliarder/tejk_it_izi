#pragma once

#include <boost/asio.hpp>
#include <logger.hpp>
#include <loginserver_data.hpp>
#include <unordered_set>

class gameserver_session : public std::enable_shared_from_this<gameserver_session>,
                           public gameserver_write_base
{
  boost::asio::ip::tcp::socket&& sock_;
  logger& logger_;
  std::array<char, 2048> recvbuf_;
  loginserver_data& login_data_;
  std::shared_ptr<gameserver> associated_gs_;

  void do_recv_header();
  void do_recv_data(unsigned expected_len);
  virtual void request_kick_player(const std::string & name) override;
  void write(const std::array<char, 2048>& data, unsigned size);

  void handle_packet(unsigned len);

public:
  gameserver_session(boost::asio::ip::tcp::socket&& sock,
                     logger& logger,
                     loginserver_data& login_data);
  ~gameserver_session();

  void start();
};
