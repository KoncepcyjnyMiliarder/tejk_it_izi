#pragma once

#include <list>
#include <string>

// this class is a very simplified representation of world character
// its used only on character selection screen in lobby state
// so it doesnt need to know inventory, world position etc etc

struct lobby_character
{
  const unsigned uid;
  const std::string name;

  lobby_character(unsigned uid, std::string name)
    : uid(uid)
    , name(std::move(name))
  {
  }

  using lobby_character_list = std::list<lobby_character>;
};