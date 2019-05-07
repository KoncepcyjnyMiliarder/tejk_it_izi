#pragma once

#include <lobby_character.hpp>
#include <account_data.hpp>

class database_facade
{
  public:

    virtual lobby_character::lobby_character_list get_lobby_chars(unsigned account_uid) = 0;
    virtual void erase_character(const lobby_character& character) = 0;
    virtual bool is_nickname_already_taken(const std::string& nickname) = 0;
    virtual lobby_character create_character(const std::string& nickname, unsigned owner_acc_id) = 0;
    virtual account_data get_account_data(const std::string& login) = 0;
    ~database_facade() = default;
};