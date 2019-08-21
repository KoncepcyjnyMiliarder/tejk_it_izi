#include <world.hpp>

world::world(database_facade& db, task_scheduler& scheduler)
  : scheduler_(scheduler),
    db_(db),
    online_player_registry_(db),
    chat_(scheduler_, online_player_registry_),
    friends_(online_player_registry_, clock_, db, scheduler)
{
}
