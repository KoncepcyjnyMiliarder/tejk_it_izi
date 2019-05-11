#pragma once

#include <received_packet.hpp>
#include <binary_deserializer.hpp>
#include <user_environment.hpp>

class send_chat_msg
  : public received_packet
{
    user_environment& my_environment_;
    std::string chatroom_name_;
    std::string message_;

  public:

    send_chat_msg(binary_deserializer& bd, user_environment& my_environment);

    virtual void execute_associated_action() override;
};