#pragma once

#include <chat_system.hpp>
#include <chat_participant.hpp>

//to be instantiated in mmoclient's ingame_state
//responsible for handling channel chat packets
//and proper cleanup/unregistration on client destruction
class chat_controller
{

    chat_system<chat_participant>& chat_sys_;
    chat_participant my_representative_;
    std::list<chat_system<chat_participant>::chatroom_ptr> chatrooms_im_in_;

  public:

    chat_controller(chat_system<chat_participant>& chat_sys, std::shared_ptr<net_session> my_session, const std::string& my_name);
    ~chat_controller();

    void register_me(const std::string& chatroom_name);
    void unregister_me(const std::string& chatroom_name);
    void broadcast_message(const std::string& chatroom_name, const std::string& msg);
    std::size_t count_of_chatrooms_im_in() const;
};