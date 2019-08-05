#include <boost_socket.hpp>

void boost_socket::do_recv_expected_len(recv_completion_handler& handler)
{
  auto self = shared_from_this();
  async_read(sock_, boost::asio::buffer(recvbuf_, 4), sync_strand_.wrap([this, &handler, self](const boost::system::error_code & ec, std::size_t bytes_read)
  {
    //i am not entirely sure if this is OK
    //handlers may be dispatched as my session object holding reference to this socket is already dead
    //so i wrap this little thing in shared ptr and check the flag? is that it? rip in peppers, hope it does not explode
    if (is_pending_deletion_)
      return;
    unsigned len = *(unsigned*)recvbuf_.data(); // duh
    if (ec || bytes_read != 4 || len > recvbuf_.size())
    {
      handler.on_error();
      return;
    }
    do_recv_data(handler, len);
  }));
}

void boost_socket::do_recv_data(recv_completion_handler& handler, unsigned expected_len)
{
  auto self = shared_from_this();
  async_read(sock_, boost::asio::buffer(recvbuf_, expected_len), sync_strand_.wrap([this, &handler, self, expected_len](const boost::system::error_code & ec, std::size_t bytes_read)
  {
    //same concerns as above
    if (is_pending_deletion_)
      return;
    if (ec || bytes_read != expected_len)
    {
      handler.on_error();
      return;
    }
    handler.on_recv(recvbuf_, bytes_read);
  }));
}

boost_socket::boost_socket(boost::asio::ip::tcp::socket&& sock, boost::asio::strand& sync_strand)
  : is_pending_deletion_(false),
    sock_(std::move(sock)),
    sync_strand_(sync_strand)
{
}

void boost_socket::async_recv(recv_completion_handler& handler)
{
  sync_strand_.post(std::bind(&boost_socket::do_recv_expected_len, this, std::ref(handler)));
}

void boost_socket::send_to_client(const buffer& data, unsigned size)
{
  boost::system::error_code ec; //TODO, what on error?
  boost::asio::write(sock_, boost::asio::buffer((const char*)&size, sizeof(unsigned)), ec);
  boost::asio::write(sock_, boost::asio::buffer(data.data(), size), ec);
}

void boost_socket::force_close()
{
  if (is_pending_deletion_)
    return;
  is_pending_deletion_ = true;
  boost::system::error_code ec; //TODO, what on error?
  sock_.close(ec);
}
