#pragma once

#include <chat_system.hpp>
#include <chat_participant.hpp>
#include <world_registrar.hpp>
#include <tii_player_character.hpp>
#include <friend_system.hpp>
#include <friendlist_backend.hpp>
#include <task_scheduler.hpp>

//very temp clock impl
#include <chrono>
struct realtime_clock
{
  unsigned get_current_ms_stamp() //stackoverflow
  {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  }
};

class world
  : public world_registrar<tii_player_character>
{
    task_scheduler& scheduler_;
    realtime_clock clock_;
    database_facade& db_;

  public:

    using online_players_registrar = world_registrar<tii_player_character>;
    using chat_system = chat_system_base<chat_participant>;
    using friend_system = friend_system_base<online_players_registrar, friendlist_backend, realtime_clock>;

    chat_system chat_;
    friend_system friends_;

    world(database_facade& db, task_scheduler& scheduler);
};