#pragma once

#include <received_packet.hpp>
#include <state_transitioner.hpp>
#include <user_environment.hpp>

class ingame_state_recv_packet_factory
{
  state_transitioner& transitioner_;
  user_environment& my_environment_;

public:
  ingame_state_recv_packet_factory(state_transitioner& transitioner,
                                   user_environment& my_environment);
  received_packet::packet_ptr construct(const std::array<char, 2048>& data, unsigned size);
};