#pragma once

#include <net_session.hpp>
#include <chat_system/chat_backend.hpp>

class default_chat_backend
  : public chat_backend
{
  private:

    const net_session::session_ptr my_session_;

  public:

    default_chat_backend(net_session::session_ptr my_session);

    virtual void on_chat_message(const std::string& channel_name, const std::string& who, const std::string& message) override;
    virtual void on_user_left_chat(const std::string& channel_name, const std::string& who) override;
    virtual void on_user_joined_chat(const std::string& channel_name, const std::string& who) override;
    virtual void on_whisper(const std::string& who, const std::string& message) override;
};