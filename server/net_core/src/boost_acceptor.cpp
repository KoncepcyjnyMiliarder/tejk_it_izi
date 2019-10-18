#include <boost_acceptor.hpp>
#include <boost_socket.hpp>

boost_acceptor::boost_acceptor(
  boost::asio::ip::tcp::endpoint ep,
  boost::asio::strand<boost::asio::io_context::executor_type>& sync_strand)
  : acceptor_(sync_strand, ep)
  , sock_(sync_strand)
{
}

void
boost_acceptor::async_accept(accept_completion_handler& handler)
{
  acceptor_.async_accept(sock_, [this, &handler](const boost::system::error_code& ec) {
    if (!ec)
      handler.on_accept(std::make_unique<boost_socket>(std::move(sock_)));
    async_accept(handler);
  });
}
