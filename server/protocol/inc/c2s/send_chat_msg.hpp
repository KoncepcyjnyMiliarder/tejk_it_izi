#pragma once

#include <received_packet.hpp>
#include <binary_deserializer.hpp>
#include <net_session.hpp>
#include <logger.hpp>
#include <world.hpp>

class send_chat_msg
  : public received_packet
{
    std::shared_ptr<net_session> my_session_;
    logger& logger_;
    world::chat_system& chat_sys_;
    std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in_;
    std::string chatroom_name_;
    std::string message_;

  public:

    send_chat_msg(binary_deserializer& bd, std::shared_ptr<net_session> my_session,
                  logger& logger, world::chat_system& chat_sys,
                  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in);

    virtual void execute_associated_action() override;
};