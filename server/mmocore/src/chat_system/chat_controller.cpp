#include <chat_system/chat_controller.hpp>
#include <chat_system/default_chat_backend.hpp>

chat_controller::chat_controller(tii_entity_representative::reference my_representative, chat_system& chat_sys)
  : my_representative_(my_representative),
    chat_sys_(chat_sys)
{
  chat_sys_.register_me(my_representative_, std::make_unique<default_chat_backend>(my_representative_.my_session()));
}

chat_controller::~chat_controller()
{
  chat_sys_.unregister_me(my_representative_);
}

void chat_controller::register_me(const std::string& chatroom_name)
{
  chat_sys_.join_chatroom(my_representative_, chatroom_name);
}

void chat_controller::unregister_me(const std::string& chatroom_name)
{
  chat_sys_.leave_chatroom(my_representative_, chatroom_name);
}

void chat_controller::broadcast_message(const std::string& chatroom_name, const std::string& msg)
{
  chat_sys_.broadcast_message(my_representative_, chatroom_name, msg);
}

void chat_controller::send_whisper(const std::string& recipient_nick, const std::string& msg)
{
  //TODO!
}
