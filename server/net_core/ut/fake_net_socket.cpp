#include <fake_net_socket.hpp>
#include <stdexcept>

fake_net_socket::fake_net_socket(unsigned id)
  : sock_id_(id),
    handler_(nullptr),
    is_open_(true)
{
}

void fake_net_socket::async_recv(recv_completion_handler& handler)
{
  handler_ = &handler;
}

void fake_net_socket::send_to_client(const buffer& data, unsigned size)
{
  if (is_open_)
    packets_sent_to_client_.push(std::string(data.data(), size));
}

void fake_net_socket::force_close()
{
  is_open_ = false;
}

void fake_net_socket::simulate_recv(const net_socket::buffer& data, unsigned size)
{
  if (!handler_)
    throw std::logic_error("async_recv has not been called yet");
  if (is_open_)
    handler_->on_recv(data, size);
}

std::queue<std::string>& fake_net_socket::get_packets_sent_to_client()
{
  return packets_sent_to_client_;
}

bool fake_net_socket::is_open()
{
  return is_open_;
}
