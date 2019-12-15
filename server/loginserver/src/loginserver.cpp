#include <client_session.hpp>
#include <gameserver_session.hpp>
#include <loginserver.hpp>

void
loginserver::do_accept_client()
{
  client_acceptor_.async_accept(client_socket_, [this](const boost::system::error_code& ec) {
    if (!ec)
      std::make_shared<client_session>(std::move(client_socket_), logger_, login_data_)->start();
    do_accept_client();
  });
}

void
loginserver::do_accept_gs()
{
  gs_acceptor_.async_accept(gs_socket_, [this](const boost::system::error_code& ec) {
    if (!ec)
      std::make_shared<gameserver_session>(std::move(client_socket_), logger_, login_data_)
        ->start();
    do_accept_gs();
  });
}

loginserver::loginserver(boost::asio::ip::tcp::acceptor& client_acceptor,
                         boost::asio::ip::tcp::acceptor& gs_acceptor)
  : client_acceptor_(client_acceptor)
  , gs_acceptor_(gs_acceptor)
  , client_socket_(client_acceptor_.get_executor())
  , gs_socket_(gs_acceptor_.get_executor())
{
}

void
loginserver::run()
{
  logger_.log_diagnostic(__func__);
  do_accept_client();
  do_accept_gs();
}
