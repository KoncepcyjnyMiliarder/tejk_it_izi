#pragma once

#include <received_packet.hpp>
#include <binary_deserializer.hpp>
#include <logger.hpp>
#include <world.hpp>

class leave_chat
  : public received_packet
{
    logger& logger_;
    std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in_;
    std::string chatroom_name_;

  public:

    leave_chat(binary_deserializer& bd, logger& logger,
               std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in);

    virtual void execute_associated_action() override;
};