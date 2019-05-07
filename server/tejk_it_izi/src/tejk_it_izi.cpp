#include <tejk_it_izi.hpp>
#include <mmoclient.hpp>

tejk_it_izi::tejk_it_izi(database_facade& db, std::unique_ptr<client_acceptor> acceptor)
  : db_(db),
    acceptor_(std::move(acceptor))
{
  acceptor_->async_accept(*this);
}

void tejk_it_izi::tick()
{
  while (!never_never_land_.empty())
    never_never_land_.pop();
}

void tejk_it_izi::on_accept(std::shared_ptr<net_socket> sock)
{
  std::make_shared<mmoclient>(std::move(sock), db_, never_never_land_)->start(universe_, logger_, authenticator_);
}
