#include <mmoclient.hpp>
#include <preauth_state.hpp>
#include <null_socket.hpp>
#include <null_state.hpp>

void mmoclient::on_recv(const net_socket::buffer& data, unsigned size)
{
  current_state_->handle_network_packet(data, size);
}

void mmoclient::on_error()
{
  force_close();
}

mmoclient::mmoclient(std::shared_ptr<net_socket> sock, database_facade& db, std::queue<std::unique_ptr<client_state>>& never_never_land)
  : net_session(std::move(sock)),
    db_(db),
    never_never_land_(never_never_land)
{
}

void mmoclient::start(world& universe, logger& logger, login_validator& authenticator)
{
  current_state_ = std::make_unique<preauth_state>(*this, shared_from_this(), universe, logger, authenticator, db_);
  current_state_->start();
  do_recv();
}

void mmoclient::force_close()
{
  my_sock_->force_close();
  my_sock_ = std::make_unique<null_socket>();
  never_never_land_.push(std::move(current_state_));
  current_state_ = std::make_unique<null_state>();
}
