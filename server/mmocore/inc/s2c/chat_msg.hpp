#pragma once

#include <net_socket.hpp>

namespace ingame_packet_constructors
{
unsigned chat_msg(net_socket::buffer& buffer_to_fill,
                  const std::string& chatroom,
                  const std::string& user,
                  const std::string& msg);
}