#include <world.hpp>

world::world(database_facade& db)
  : world_registrar<tii_player_character>(db),
    db_(db),
    friends_(*this, clock_, db)
{
}
