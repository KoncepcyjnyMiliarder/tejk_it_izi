#pragma once

#include <client_acceptor.hpp>
#include <fake_net_socket.hpp>

class fake_client_acceptor
  : client_acceptor
{
    unsigned last_socket_id_;
    accept_completion_handler* handler_;

  public:

    fake_client_acceptor();

    virtual void async_accept(accept_completion_handler& handler) override;

    void simlulate_socket_spawn(unsigned count);
};