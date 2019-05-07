#pragma once

#include <string>

class login_validator
{
  public:
    //jakis token return?
    virtual bool are_loginkey_and_username_valid(const std::string& loginkey, const std::string& login) = 0;
    virtual ~login_validator() = default;
};