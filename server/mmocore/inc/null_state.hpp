#pragma once

#include <client_state.hpp>

class null_state :
  public client_state
{
    virtual void handle_network_packet(const std::array<char, 2048>& data, unsigned len) override {}
    virtual void start() override {}
};