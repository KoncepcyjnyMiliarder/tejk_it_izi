#pragma once

#include <net_socket.hpp>

namespace ingame_packet_constructors
{
unsigned moved_to_lobby(net_socket::buffer& buffer_to_fill);
}