#pragma once

#include <received_packet.hpp>
#include <binary_deserializer.hpp>
#include <lobby_character.hpp>
#include <net_session.hpp>
#include <state_transitioner.hpp>
#include <world.hpp>
#include <logger.hpp>
#include <database_facade.hpp>

class request_character_select
  : public received_packet
{
    std::shared_ptr<net_session> my_session_;
    lobby_character::lobby_character_list& lobby_chars_;
    state_transitioner& transitioner_;
    logger& logger_;
    world& world_;
    std::string char_name_;
    database_facade& db_;
    account_data& acc_data_;

  public:

    request_character_select(binary_deserializer& bd, std::shared_ptr<net_session> my_session, state_transitioner& transitioner,
                             world& universe, logger& logger, lobby_character::lobby_character_list& lobby_chars,
                             database_facade& db, account_data& acc_data);
    virtual void execute_associated_action() override;
};