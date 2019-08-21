#pragma once

#include <chat_system/chat_system.hpp>

class chat_controller
{
    tii_entity_representative::reference my_representative_;
    chat_system& chat_sys_;

  public:

    chat_controller(tii_entity_representative::reference my_representative, chat_system& chat_sys);
    ~chat_controller();

    void register_me(const std::string& chatroom_name);
    void unregister_me(const std::string& chatroom_name);
    void broadcast_message(const std::string& chatroom_name, const std::string& msg);
    void send_whisper(const std::string& recipient_nick, const std::string& msg);
};