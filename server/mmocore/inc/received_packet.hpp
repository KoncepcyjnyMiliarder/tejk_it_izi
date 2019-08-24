#pragma once

#include <memory>

class received_packet
{
public:
  using packet_ptr = std::unique_ptr<received_packet>;

  virtual void execute_associated_action() = 0;
  virtual ~received_packet() = default;
};