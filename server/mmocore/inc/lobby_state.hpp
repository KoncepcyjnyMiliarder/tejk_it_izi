#pragma once

#include <asynchronous_database_adapter.hpp>
#include <lobby_character.hpp>
#include <lobby_state_recv_packet_factory.hpp>
#include <logger.hpp>
#include <state_transitioner.hpp>
#include <world.hpp>

class lobby_state : public client_state
{
  state_transitioner& transitioner_;
  std::shared_ptr<net_session> my_session_;
  world& world_;
  logger& logger_;
  database_facade& db_;
  asynchronous_database_adapter& async_db_;
  account_data acc_data_;

  lobby_state_recv_packet_factory recv_factory_;
  lobby_character::lobby_character_list lobby_chars_;

public:
  lobby_state(state_transitioner& transitioner,
              std::shared_ptr<net_session> my_session,
              world& universe,
              logger& logger,
              database_facade& db,
              account_data acc_data,
              asynchronous_database_adapter& async_db);
  virtual void handle_network_packet(const std::array<char, 2048>& data, unsigned len) override;
  virtual void start() override;

  ~lobby_state() = default;
};