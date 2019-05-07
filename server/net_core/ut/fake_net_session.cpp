#include <fake_net_session.hpp>
#include <algorithm>

fake_net_session::fake_net_session(std::shared_ptr<net_socket> sock)
  : net_session(sock)
{
  do_recv();
}

void fake_net_session::on_recv(const net_socket::buffer& data, unsigned size)
{
}

void fake_net_session::on_error()
{
  force_close();
}

void fake_net_session::force_close()
{
  my_sock_->force_close();
}
