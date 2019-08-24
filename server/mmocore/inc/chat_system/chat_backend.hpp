#pragma once

#include <string>

class chat_backend
{
public:
  virtual void on_chat_message(const std::string& channel_name,
                               const std::string& who,
                               const std::string& message) = 0;
  virtual void on_user_left_chat(const std::string& channel_name, const std::string& who) = 0;
  virtual void on_user_joined_chat(const std::string& channel_name, const std::string& who) = 0;
  virtual void on_whisper(const std::string& who, const std::string& message) = 0;

  virtual ~chat_backend() = default;
};