#pragma once

#include <chat_system.hpp>
#include <playable_character.hpp>

class world
{
  public:

    using player_pawn = playable_character<world>;
    using chat_system = chat_subsystem<player_pawn>;

    chat_system chat_;
};