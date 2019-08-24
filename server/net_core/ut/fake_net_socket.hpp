#pragma once

#include <net_socket.hpp>
#include <queue>

class fake_net_socket : public net_socket
{
  const unsigned sock_id_;
  recv_completion_handler* handler_;
  bool is_open_;
  std::queue<std::string> packets_sent_to_client_;

public:
  fake_net_socket(unsigned id = 1337);

  virtual void async_recv(recv_completion_handler& handler) override;
  virtual void send_to_client(const buffer& data, unsigned size) override;
  virtual void force_close() override;
  void simulate_recv(const net_socket::buffer& data, unsigned size);

  std::queue<std::string>& get_packets_sent_to_client();
  bool is_open();
};