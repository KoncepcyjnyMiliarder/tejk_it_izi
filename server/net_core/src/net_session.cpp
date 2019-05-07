#include <net_session.hpp>

void net_session::do_recv()
{
  my_sock_->async_recv(*this);
}

net_session::net_session(std::shared_ptr<net_socket> sock)
  : my_sock_(std::move(sock))
{
}

void net_session::send_to_client(const net_socket::buffer& data, unsigned size)
{
  my_sock_->send_to_client(data, size);
}
