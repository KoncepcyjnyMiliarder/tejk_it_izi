#pragma once

#include <received_packet.hpp>
#include <net_session.hpp>
#include <state_transitioner.hpp>
#include <world.hpp>
#include <logger.hpp>
#include <database_facade.hpp>
#include <account_data.hpp>

class ingame_state_recv_packet_factory
{
    std::shared_ptr<net_session> my_session_;
    state_transitioner& transitioner_;
    world& world_;
    logger& logger_;
    database_facade& db_;
    world::player_pawn& my_pawn_;
    account_data& acc_data_;
    std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in_;

  public:

    ingame_state_recv_packet_factory(std::shared_ptr<net_session> my_session, state_transitioner& transitioner, world& universe,
                                     logger& logger, database_facade& db, world::player_pawn& my_pawn, account_data& acc_data,
                                     std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in);
    received_packet::packet_ptr construct(const std::array<char, 2048>& data, unsigned size);
};