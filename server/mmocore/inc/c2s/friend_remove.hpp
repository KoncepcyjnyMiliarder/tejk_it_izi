#pragma once

#include <binary_deserializer.hpp>
#include <received_packet.hpp>
#include <user_environment.hpp>

class friend_remove : public received_packet
{
  user_environment& my_environment_;
  std::string friend_name_;

public:
  friend_remove(binary_deserializer& bd, user_environment& my_environment);

  virtual void execute_associated_action() override;
};