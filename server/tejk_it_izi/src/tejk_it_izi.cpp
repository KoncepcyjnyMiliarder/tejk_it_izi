#include <tejk_it_izi.hpp>
#include <mmoclient.hpp>
#include <iostream>
#include <thread>

tejk_it_izi::tejk_it_izi(database_facade& db, std::unique_ptr<client_acceptor> acceptor,
                         boost::asio::deadline_timer& timer, boost::asio::strand& sync_strand)
  : db_(db),
    acceptor_(std::move(acceptor)),
    timer_(timer),
    sync_strand_(sync_strand),
    scheduler_(sync_strand_),
    start_(std::chrono::steady_clock::now()),
    universe_(db)
{
  acceptor_->async_accept(*this);
}

void tejk_it_izi::tick()
{
  timer_.expires_at(timer_.expires_at() + boost::posix_time::microseconds(16666));
  timer_.async_wait(sync_strand_.wrap(std::bind(&tejk_it_izi::tick, this)));
}

void tejk_it_izi::run()
{
  timer_.async_wait(sync_strand_.wrap(std::bind(&tejk_it_izi::tick, this)));
}

void tejk_it_izi::on_accept(std::shared_ptr<net_socket> sock)
{
  std::make_shared<mmoclient>(std::move(sock), db_, scheduler_)->start(universe_, logger_, authenticator_);
}
