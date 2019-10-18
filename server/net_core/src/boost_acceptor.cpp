#include <boost_acceptor.hpp>
#include <boost_socket.hpp>

boost_acceptor::boost_acceptor(boost::asio::io_service& io_service,
                               boost::asio::ip::tcp::endpoint ep,
                               boost::asio::strand<boost::asio::io_context::executor_type>& sync_strand)
  : io_service_(io_service)
  , acceptor_(io_service, ep)
  , sock_(io_service_)
  , sync_strand_(sync_strand)
{
}

void
boost_acceptor::async_accept(accept_completion_handler& handler)
{
  acceptor_.async_accept(
    sock_, boost::asio::bind_executor(sync_strand_, [this, &handler](const boost::system::error_code& ec) {
      if (!ec)
        handler.on_accept(std::make_unique<boost_socket>(std::move(sock_), sync_strand_));
      async_accept(handler);
    }));
}
