#pragma once

#include <binary_deserializer.hpp>
#include <received_packet.hpp>
#include <user_environment.hpp>

class send_whisper : public received_packet
{
  user_environment& my_environment_;
  std::string recipient_;
  std::string message_;

public:
  send_whisper(binary_deserializer& bd, user_environment& my_environment);

  virtual void execute_associated_action() override;
};