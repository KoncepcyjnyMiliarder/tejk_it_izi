#include <fake_client_acceptor.hpp>

fake_client_acceptor::fake_client_acceptor()
  : last_socket_id_(0)
{
}

void
fake_client_acceptor::async_accept(accept_completion_handler& handler)
{
  handler_ = &handler;
}

void
fake_client_acceptor::simlulate_socket_spawn(unsigned count)
{
  if (!handler_)
    throw std::logic_error("async_accept has not been called yet");
  while (count--)
  {
    handler_->on_accept(std::make_unique<fake_net_socket>(last_socket_id_));
    ++last_socket_id_;
  }
}
