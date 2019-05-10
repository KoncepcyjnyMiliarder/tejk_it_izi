#include <tii_player_character.hpp>

tii_player_character::tii_player_character(const lobby_character& selected_char,
    registrar_t& online_players_registrar, std::shared_ptr<net_session> my_session)
  : id_(selected_char.uid),
    name_(selected_char.name),
    online_players_registrar_(online_players_registrar),
    my_session_(my_session)
{
  online_players_registrar_.register_me(*this);
}

tii_player_character::~tii_player_character()
{
  online_players_registrar_.unregister_me(*this);
}

unsigned tii_player_character::uid() const
{
  return id_;
}

const std::string& tii_player_character::name() const
{
  return name_;
}
