#pragma once

#include <net_socket.hpp>

namespace ingame_packet_constructors
{
unsigned friend_leave(net_socket::buffer& buffer_to_fill, const std::string& name);
}