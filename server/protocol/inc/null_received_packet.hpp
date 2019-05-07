#pragma once

#include <received_packet.hpp>
#include <binary_deserializer.hpp>

class null_received_packet
  : public received_packet
{
  public:

    null_received_packet(/*moze cos co wie czy podczas zlego pakietu wyjebac gracza,olac czy cos?*/);
    virtual void execute_associated_action() override;
};