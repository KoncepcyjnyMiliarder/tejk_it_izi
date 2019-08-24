#pragma once

#include <net_socket.hpp>

namespace ingame_packet_constructors
{
unsigned user_joined_chat(net_socket::buffer& buffer_to_fill,
                          const std::string& chatroom,
                          const std::string& user);
}