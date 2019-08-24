#pragma once

#include <friend_system_error_codes.hpp>
#include <net_socket.hpp>

namespace ingame_packet_constructors
{
unsigned friend_add_result(net_socket::buffer& buffer_to_fill,
                           friend_add_request_result result,
                           const std::string& name);
}