#pragma once

#include <net_session.hpp>
#include <world_registrar.hpp>

class tii_player_character
{
  public:

    using pointer = tii_player_character*;
    using const_pointer = const tii_player_character*;
    using reference = tii_player_character&;
    using const_reference = const tii_player_character&;
    using registrar_t = typename world_registrar<tii_player_character>;

  private:

    const unsigned id_;
    const std::string name_;
    registrar_t& online_players_registrar_;
    std::shared_ptr<net_session> my_session_;

  public:

    tii_player_character(const lobby_character& selected_char, registrar_t& online_players_registrar,
                         std::shared_ptr<net_session> my_session); //todo model of full world info
    ~tii_player_character();

    unsigned uid() const;
    const std::string& name() const;
};