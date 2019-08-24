#include <c2s/friend_add.hpp>

friend_add::friend_add(binary_deserializer& bd, user_environment& my_environment)
  : my_environment_(my_environment)
{
  bd >> friend_name_;
}

void
friend_add::execute_associated_action()
{
  my_environment_.friends_controller_.on_req_friend_add(friend_name_);
}
