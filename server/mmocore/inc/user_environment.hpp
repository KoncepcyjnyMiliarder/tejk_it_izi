#pragma once

#include <net_session.hpp>
#include <chat_system/chat_controller.hpp>
#include <logger.hpp>
#include <account_data.hpp>
#include <world.hpp>
#include <friend_system/friend_controller.hpp>
#include <asynchronous_database_adapter.hpp>

//this class is intended to act as a "bag" for members of ingame_state,
//so it is not necessary to pass every piece of my class to every component
//but just let the components take environment and fetch what the need
//maybe it violates ISP but it's very convenient
struct user_environment
{

  std::shared_ptr<net_session> my_session_;
  world& universe_;
  tii_entity_representative my_character_;
  chat_controller chat_controller_;
  friend_controller friends_controller_;
  logger& logger_;
  database_facade& db_;
  account_data acc_data_;
  asynchronous_database_adapter& async_db_;

  user_environment(std::shared_ptr<net_session> my_session, world& universe, const lobby_character& selected_char,
                   logger& logger,  database_facade& db, account_data acc_data, asynchronous_database_adapter& async_db)
    : my_session_(my_session), universe_(universe), my_character_(selected_char, my_session),
      chat_controller_(my_character_, universe.chat_),
      friends_controller_(my_character_, universe.friends_),
      logger_(logger),  db_(db), acc_data_(std::move(acc_data)), async_db_(async_db) {}

};