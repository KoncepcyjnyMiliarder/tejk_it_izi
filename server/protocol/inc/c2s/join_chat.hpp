#pragma once

#include <received_packet.hpp>
#include <binary_deserializer.hpp>
#include <user_environment.hpp>

class join_chat
  : public received_packet
{
    user_environment& my_environment_;
    std::string chatroom_name_;

  public:

    join_chat(binary_deserializer& bd, user_environment& my_environment);

    virtual void execute_associated_action() override;
};