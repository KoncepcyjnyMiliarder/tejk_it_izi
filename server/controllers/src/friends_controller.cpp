#include <friends_controller.hpp>

friends_controller::friends_controller(representative_ref& my_representative, world::friend_system& fs)
  : my_representative_(my_representative),
    fs_(fs)
{
  fs_.register_me(my_representative_);
}

friends_controller::~friends_controller()
{
  fs_.unregister_me(my_representative_);
}

void friends_controller::on_req_friend_add(const std::string& name)
{
  fs_.process_req_friend_add(my_representative_, name);
}

void friends_controller::on_req_friend_remove(const std::string& name)
{
  fs_.process_req_friend_remove(my_representative_, name);
}