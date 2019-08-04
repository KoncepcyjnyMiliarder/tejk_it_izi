#pragma once

#include <client_acceptor.hpp>
#include <boost/asio.hpp>

class boost_acceptor
  : public client_acceptor
{

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket sock_;
    boost::asio::strand& sync_strand_;

  public:

    boost_acceptor(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint ep, boost::asio::strand& sync_strand);

    virtual void async_accept(accept_completion_handler& handler) override;
};
