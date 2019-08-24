#pragma once

#include <asynchronous_database_adapter.hpp>
#include <logger.hpp>
#include <login_validator.hpp>
#include <net_session.hpp>
#include <state_transitioner.hpp>
#include <world.hpp>

class preauth_state : public client_state
{
  state_transitioner& transitioner_;
  std::shared_ptr<net_session> my_session_;
  world& world_;
  logger& logger_;
  login_validator& authenticator_;
  database_facade& db_;
  asynchronous_database_adapter& async_db_;

  static std::array<char, 2048> ok_response_;
  static std::array<char, 2048> fuck_you_response_;

public:
  preauth_state(state_transitioner& transitioner,
                std::shared_ptr<net_session> my_session,
                world& universe,
                logger& logger,
                login_validator& authenticator,
                database_facade& db,
                asynchronous_database_adapter& async_db);
  ~preauth_state() = default;

  virtual void handle_network_packet(const std::array<char, 2048>& data, unsigned len) override;
  virtual void start() override;
};