#pragma once

#include <net_session.hpp>
#include <net_socket.hpp>

class fake_net_session
  : public net_session
{

  public:

    fake_net_session(std::shared_ptr<net_socket> sock);

    virtual void on_recv(const net_socket::buffer& data, unsigned size) override;
    virtual void on_error() override;
    virtual void force_close() override;
};