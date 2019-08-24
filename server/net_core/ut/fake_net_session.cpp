#include <algorithm>
#include <fake_net_session.hpp>

fake_net_session::fake_net_session(std::shared_ptr<fake_net_socket> sock)
  : net_session(sock)
  , sock_(sock)
{
  do_recv();
}

void
fake_net_session::on_recv(const net_socket::buffer& data, unsigned size)
{
}

void
fake_net_session::on_error()
{
  force_close();
}

void
fake_net_session::force_close()
{
  my_sock_->force_close();
}

std::shared_ptr<fake_net_socket>
fake_net_session::get_sock()
{
  return sock_;
}
