#pragma once

#include <user_environment.hpp>
#include <fake_logger.hpp>
#include <fake_database_facade.hpp>
#include <fake_net_session.hpp>
#include <memory>

struct fake_user_environment
{
  fake_logger logger;
  fake_database_facade db;
  world unvierse;

  using env_session_pair = std::pair<std::unique_ptr<user_environment>, std::shared_ptr<fake_net_session>>;
  env_session_pair make_env(const std::string& account_login, const std::string& character_nickname);
};