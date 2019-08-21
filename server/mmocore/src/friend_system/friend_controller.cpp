#include <friend_system/friend_controller.hpp>
#include <friend_system/default_friend_backend.hpp>

friend_controller::friend_controller(tii_entity_representative::reference my_representative, friend_system& fs)
  : my_representative_(my_representative),
    fs_(fs)
{
  fs_.register_me(my_representative_, std::make_unique<default_friend_backend>(my_representative.my_session()));
}

friend_controller::~friend_controller()
{
  fs_.unregister_me(my_representative_);
}

void friend_controller::on_req_friend_add(const std::string& name)
{
  fs_.process_req_friend_add(my_representative_, name);
}

void friend_controller::on_req_friend_remove(const std::string& name)
{
  fs_.process_req_friend_remove(my_representative_, name);
}