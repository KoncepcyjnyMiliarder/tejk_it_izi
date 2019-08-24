#pragma once

#include <binary_deserializer.hpp>
#include <received_packet.hpp>
#include <user_environment.hpp>

class leave_chat : public received_packet
{
  user_environment& my_environment_;
  std::string chatroom_name_;

public:
  leave_chat(binary_deserializer& bd, user_environment& my_environment);

  virtual void execute_associated_action() override;
};