#include <chat_system/default_chat_backend.hpp>
#include <s2c/chat_msg.hpp>
#include <s2c/user_left_chat.hpp>
#include <s2c/user_joined_chat.hpp>

default_chat_backend::default_chat_backend(net_session::session_ptr my_session)
  : my_session_(my_session)
{
}

void default_chat_backend::on_chat_message(const std::string& channel_name, const std::string& who, const std::string& message)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::chat_msg(buffer, channel_name, who, message);
  my_session_->send_to_client(buffer, len);
}

void default_chat_backend::on_user_left_chat(const std::string& channel_name, const std::string& who)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::user_left_chat(buffer, channel_name, who);
  my_session_->send_to_client(buffer, len);
}

void default_chat_backend::on_user_joined_chat(const std::string& channel_name, const std::string& who)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::user_joined_chat(buffer, channel_name, who);
  my_session_->send_to_client(buffer, len);
}

void default_chat_backend::on_whisper(const std::string& who, const std::string& message)
{
  //todo
}
