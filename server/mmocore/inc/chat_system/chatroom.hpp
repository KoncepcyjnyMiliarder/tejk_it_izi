#pragma once

#include <tii_entity_representative.hpp>
#include <unordered_set>

struct chatroom
{
  const std::string room_name;
  std::unordered_set<tii_entity_representative::const_pointer> participants;

  chatroom(const std::string& name)
    : room_name(name)
  {
  }
};