#pragma once

#include <received_packet.hpp>
#include <binary_deserializer.hpp>
#include <user_environment.hpp>

class friend_add
  : public received_packet
{
    user_environment& my_environment_;
    std::string friend_name_;

  public:

    friend_add(binary_deserializer& bd, user_environment& my_environment);

    virtual void execute_associated_action() override;
};