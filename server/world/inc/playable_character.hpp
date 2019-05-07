#pragma once

#include <net_session.hpp>
#include <s2c/user_joined_chat.hpp>
#include <s2c/user_left_chat.hpp>
#include <s2c/chat_msg.hpp>

template <typename PlayerRegistrar>
class playable_character
{
    PlayerRegistrar& registrar_;
    std::shared_ptr<net_session> my_session_;
    std::string temp_name_; //REALLY TEMP LOL

  public:

    using pointer = playable_character*;

    const std::string& user_name()
    {
      return temp_name_;
    }

    void on_chat_message(const std::string& channel_name, const std::string& who, const std::string& message)
    {
      net_socket::buffer buffer;
      auto len = ingame_packet_constructors::chat_msg(buffer, channel_name, who, message);
      my_session_->send_to_client(buffer, len);
    }

    void on_user_left_chat(const std::string& channel_name, const std::string& who)
    {
      net_socket::buffer buffer;
      auto len = ingame_packet_constructors::user_left_chat(buffer, channel_name, who);
      my_session_->send_to_client(buffer, len);
    }

    void on_user_joined_chat(const std::string& channel_name, const std::string& who)
    {
      net_socket::buffer buffer;
      auto len = ingame_packet_constructors::user_joined_chat(buffer, channel_name, who);
      my_session_->send_to_client(buffer, len);
    }

    playable_character(PlayerRegistrar& registrar, std::shared_ptr<net_session> my_session, const std::string& temp_name/*NOOO JESIO DATABASE ALE CHUJ TAM NARAZIE*/);
    ~playable_character();
};

template<typename PlayerRegistrar>
inline playable_character<PlayerRegistrar>::playable_character(PlayerRegistrar& registrar,
    std::shared_ptr<net_session> my_session, const std::string& temp_name)
  : registrar_(registrar),
    my_session_(my_session),
    temp_name_(temp_name)
{
}

template<typename PlayerRegistrar>
inline playable_character<PlayerRegistrar>::~playable_character()
{
}
