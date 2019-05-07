#pragma once

#include <array>

class net_socket
{
  public:
    using buffer = std::array<char, 2048>;

    class recv_completion_handler
    {
      public:
        virtual void on_recv(const net_socket::buffer& data, unsigned size) = 0;
        virtual void on_error() = 0;
        virtual ~recv_completion_handler() = default;
    };

    virtual void async_recv(recv_completion_handler& handler) = 0;

    virtual void send_to_client(const buffer& data, unsigned size) = 0;

    virtual void force_close() = 0;

    virtual ~net_socket() = default;
};