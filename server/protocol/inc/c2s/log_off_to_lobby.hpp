#pragma once

#include <received_packet.hpp>
#include <logger.hpp>
#include <net_session.hpp>
#include <state_transitioner.hpp>
#include <account_data.hpp>
#include <world.hpp>
#include <database_facade.hpp>

class log_off_to_lobby
  : public received_packet
{
    std::shared_ptr<net_session> my_session_;
    logger& logger_;
    state_transitioner& transitioner_;
    world& world_;
    database_facade& db_;
    account_data& acc_data_;

  public:

    log_off_to_lobby(std::shared_ptr<net_session> my_session, logger& logger, state_transitioner& transitioner,
                     world& universe, database_facade& db, account_data& acc_data);

    virtual void execute_associated_action() override;
};