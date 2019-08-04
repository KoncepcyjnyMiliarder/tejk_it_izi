#pragma once

#include <net_socket.hpp>
#include <boost/asio.hpp>

class boost_socket
  : public net_socket,
    public std::enable_shared_from_this<boost_socket>
{

    bool is_pending_deletion_;
    boost::asio::ip::tcp::socket sock_;
    boost::asio::strand& sync_strand_;
    buffer recvbuf_;

    void do_recv_expected_len(recv_completion_handler& handler);
    void do_recv_data(recv_completion_handler& handler, unsigned expected_len);

  public:

    boost_socket(boost::asio::ip::tcp::socket&& sock, boost::asio::strand& sync_strand);

    virtual void async_recv(recv_completion_handler& handler) override;
    virtual void send_to_client(const buffer& data, unsigned size) override;
    virtual void force_close() override;

};
