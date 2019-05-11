#include <c2s/friend_remove.hpp>

friend_remove::friend_remove(binary_deserializer& bd, user_environment& my_environment)
  : my_environment_(my_environment)
{
  bd >> friend_name_;
}

void friend_remove::execute_associated_action()
{
  my_environment_.friends_controller_.on_req_friend_remove(friend_name_);
}
