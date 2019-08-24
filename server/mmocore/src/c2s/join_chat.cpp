#include <c2s/join_chat.hpp>

join_chat::join_chat(binary_deserializer& bd, user_environment& my_environment)
  : my_environment_(my_environment)
{
  bd >> chatroom_name_;
}

void
join_chat::execute_associated_action()
{
  my_environment_.chat_controller_.register_me(chatroom_name_);
}
