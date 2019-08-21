#pragma once

#include <chat_system/chat_system.hpp>
#include <world_registrar.hpp>
#include <tii_entity_representative.hpp>
#include <friend_system/friend_system.hpp>
#include <task_scheduler.hpp>

class world
{
    task_scheduler& scheduler_;
    realtime_clock clock_;
    database_facade& db_;

  public:

    world_registrar online_player_registry_;
    chat_system chat_;
    friend_system friends_;

    world(database_facade& db, task_scheduler& scheduler);
};