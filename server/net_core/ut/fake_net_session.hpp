#pragma once

#include <fake_net_socket.hpp>
#include <net_session.hpp>

class fake_net_session : public net_session
{

  std::shared_ptr<fake_net_socket> sock_;

public:
  fake_net_session(std::shared_ptr<fake_net_socket> sock);

  virtual void on_recv(const net_socket::buffer& data, unsigned size) override;
  virtual void on_error() override;
  virtual void force_close() override;

  std::shared_ptr<fake_net_socket> get_sock();
};