#include <chat_participant.hpp>
#include <s2c/chat_msg.hpp>
#include <s2c/user_left_chat.hpp>
#include <s2c/user_joined_chat.hpp>

chat_participant::chat_participant(std::shared_ptr<net_session> my_session, const std::string& my_name)
  : my_session_(my_session),
    name_(my_name)
{
}

const std::string& chat_participant::user_name() const
{
  return name_;
}

void chat_participant::on_chat_message(const std::string& channel_name, const std::string& who, const std::string& message)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::chat_msg(buffer, channel_name, who, message);
  my_session_->send_to_client(buffer, len);
}

void chat_participant::on_user_left_chat(const std::string& channel_name, const std::string& who)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::user_left_chat(buffer, channel_name, who);
  my_session_->send_to_client(buffer, len);
}

void chat_participant::on_user_joined_chat(const std::string& channel_name, const std::string& who)
{
  net_socket::buffer buffer;
  auto len = ingame_packet_constructors::user_joined_chat(buffer, channel_name, who);
  my_session_->send_to_client(buffer, len);
}
