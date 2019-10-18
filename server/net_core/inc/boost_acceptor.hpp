#pragma once

#include <boost/asio.hpp>
#include <client_acceptor.hpp>

class boost_acceptor : public client_acceptor
{

  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket sock_;

public:
  boost_acceptor(boost::asio::ip::tcp::endpoint ep,
                 boost::asio::strand<boost::asio::io_context::executor_type>& sync_strand);

  virtual void async_accept(accept_completion_handler& handler) override;
};
