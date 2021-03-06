#pragma once

#include <asynchronous_database_adapter.hpp>
#include <binary_deserializer.hpp>
#include <lobby_character.hpp>
#include <logger.hpp>
#include <net_session.hpp>
#include <received_packet.hpp>
#include <state_transitioner.hpp>
#include <world.hpp>

class request_character_select : public received_packet
{
  std::shared_ptr<net_session> my_session_;
  lobby_character::lobby_character_list& lobby_chars_;
  state_transitioner& transitioner_;
  logger& logger_;
  world& world_;
  std::string char_name_;
  database_facade& db_;
  account_data& acc_data_;
  asynchronous_database_adapter& async_db_;

public:
  request_character_select(binary_deserializer& bd,
                           std::shared_ptr<net_session> my_session,
                           state_transitioner& transitioner,
                           world& universe,
                           logger& logger,
                           lobby_character::lobby_character_list& lobby_chars,
                           database_facade& db,
                           account_data& acc_data,
                           asynchronous_database_adapter& async_db);
  virtual void execute_associated_action() override;
};