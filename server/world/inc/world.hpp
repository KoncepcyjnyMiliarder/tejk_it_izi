#pragma once

#include <chat_system.hpp>
#include <chat_participant.hpp>

class world
{
  public:

    using chat_system = chat_system<chat_participant>;

    chat_system chat_;
};