#pragma once

#include <received_packet.hpp>
#include <net_session.hpp>
#include <state_transitioner.hpp>
#include <lobby_character.hpp>
#include <world.hpp>
#include <logger.hpp>
#include <database_facade.hpp>
#include <account_data.hpp>
#include <array>
#include <list>
#include <asynchronous_database_adapter.hpp>

class lobby_state_recv_packet_factory
{
    std::shared_ptr<net_session> my_session_;
    state_transitioner& transitioner_;
    world& world_;
    logger& logger_;
    lobby_character::lobby_character_list& lobby_chars_;
    database_facade& db_;
    account_data& acc_data_;
    asynchronous_database_adapter& async_db_;

  public:

    lobby_state_recv_packet_factory(std::shared_ptr<net_session> my_session, state_transitioner& transitioner, world& universe,
                                    logger& logger, lobby_character::lobby_character_list& lobby_chars,
                                    database_facade& db, account_data& acc_data, asynchronous_database_adapter& async_db);
    received_packet::packet_ptr construct(const std::array<char, 2048>& data, unsigned size);
};