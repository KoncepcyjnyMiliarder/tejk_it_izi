#pragma once

#include <net_socket.hpp>

class null_socket : public net_socket
{
  virtual void async_recv(recv_completion_handler&) override {}
  virtual void send_to_client(const buffer&, unsigned) override {}
  virtual void force_close() override {}
};
