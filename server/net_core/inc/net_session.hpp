#pragma once

#include <memory>
#include <net_socket.hpp>

class net_session : public net_socket::recv_completion_handler
{
protected:
  std::shared_ptr<net_socket> my_sock_;

public:
  using session_ptr = std::shared_ptr<net_session>;

  net_session(std::shared_ptr<net_socket> sock);

  void do_recv();

  void send_to_client(const net_socket::buffer& data, unsigned size);

  virtual ~net_session() = default;

  virtual void force_close() = 0;
};