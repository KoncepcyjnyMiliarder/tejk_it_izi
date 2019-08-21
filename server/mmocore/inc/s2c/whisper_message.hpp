#pragma once

#include <net_socket.hpp>

namespace ingame_packet_constructors
{
unsigned whisper_message(net_socket::buffer& buffer_to_fill, const std::string& sender, const std::string& msg);
}