#pragma once

#include <net_session.hpp>
#include <chat_controller.hpp>
#include <logger.hpp>
#include <database_facade.hpp>
#include <account_data.hpp>
#include <world.hpp>

//this class is intended to act as a "bag" for members of ingame_state,
//so it is not necessary to pass every piece of my class to every component
//but just let the components take environment and fetch what the need
//maybe it violates ISP but it's very convenient
struct user_environment
{

  std::shared_ptr<net_session> my_session_;
  chat_controller chat_controller_;
  logger& logger_;
  world& universe_;
  database_facade& db_;
  account_data acc_data_;

  user_environment(std::shared_ptr<net_session> my_session, chat_system<chat_participant>& chat_sys, const std::string& my_name,
                   logger& logger, world& universe, database_facade& db, account_data acc_data)
    : my_session_(my_session), chat_controller_(chat_sys, my_session, my_name),
      logger_(logger), universe_(universe), db_(db), acc_data_(std::move(acc_data)) {}

};