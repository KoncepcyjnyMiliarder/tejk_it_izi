#pragma once

#include <world.hpp>

class friend_controller
{

  tii_entity_representative::reference my_representative_;
  friend_system& fs_;

public:
  friend_controller(tii_entity_representative::reference my_representative, friend_system& fs);
  ~friend_controller();

  void on_req_friend_add(const std::string& name);
  void on_req_friend_remove(const std::string& name);
};
