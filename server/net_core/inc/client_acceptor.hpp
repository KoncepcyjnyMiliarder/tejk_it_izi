#pragma once

#include <memory>
#include <net_socket.hpp>

class client_acceptor
{
public:
  class accept_completion_handler
  {
  public:
    virtual void on_accept(std::shared_ptr<net_socket> sock) = 0;
    virtual ~accept_completion_handler() = default;
  };

  virtual void async_accept(accept_completion_handler& handler) = 0;
  virtual ~client_acceptor() = default;
};