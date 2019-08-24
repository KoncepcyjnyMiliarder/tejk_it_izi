#pragma once

#include <array>

class client_state
{

public:
  virtual void handle_network_packet(const std::array<char, 2048>& data, unsigned len) = 0;
  virtual void start() = 0;
  virtual ~client_state() = default;
};