#include <c2s/leave_chat.hpp>

leave_chat::leave_chat(binary_deserializer& bd, user_environment& my_environment)
  : my_environment_(my_environment)
{
  bd >> chatroom_name_;
}

void
leave_chat::execute_associated_action()
{
  my_environment_.chat_controller_.unregister_me(chatroom_name_);
}
