#pragma once

#include <string>

struct account_data
{
  unsigned uid;
  std::string login;
  std::string pass_hash;
  std::string salt;
  unsigned access_level;
};