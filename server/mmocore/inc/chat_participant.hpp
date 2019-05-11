#pragma once

#include <net_session.hpp>

class chat_participant
{

    std::shared_ptr<net_session> my_session_;
    std::string name_;

  public:

    chat_participant(std::shared_ptr<net_session> my_session, const std::string& my_name);

    using pointer = chat_participant*;
    using reference = chat_participant&;

    const std::string& user_name() const;
    void on_chat_message(const std::string& channel_name, const std::string& who, const std::string& message);
    void on_user_left_chat(const std::string& channel_name, const std::string& who);
    void on_user_joined_chat(const std::string& channel_name, const std::string& who);
};