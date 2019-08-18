#include <world.hpp>

world::world(database_facade& db, task_scheduler& scheduler)
  : world_registrar<tii_player_character>(db),
    scheduler_(scheduler),
    db_(db),
    chat_(scheduler_),
    friends_(*this, clock_, db)
{
}
