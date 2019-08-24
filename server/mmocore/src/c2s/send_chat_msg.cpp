#include <c2s/send_chat_msg.hpp>

send_chat_msg::send_chat_msg(binary_deserializer& bd, user_environment& my_environment)
  : my_environment_(my_environment)
{
  bd >> chatroom_name_ >> message_;
}

void
send_chat_msg::execute_associated_action()
{
  my_environment_.chat_controller_.broadcast_message(chatroom_name_, message_);
}
