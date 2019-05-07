#pragma once

#include <string>

class loginkeys_validator
{
  public:
    virtual bool are_loginkey_and_username_valid(const std::string& loginkey, const std::string& username) = 0;
};