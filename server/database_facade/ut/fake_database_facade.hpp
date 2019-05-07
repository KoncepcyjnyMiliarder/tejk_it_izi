#pragma once

#include <database_facade.hpp>
#include <map>

class fake_database_facade
  : public database_facade
{
    std::map<std::string, account_data> acc_datas_;
    std::map<unsigned, lobby_character::lobby_character_list> account_to_chars_;
    unsigned next_char_id_serial_;

  public:

    fake_database_facade();

    virtual lobby_character::lobby_character_list get_lobby_chars(unsigned account_uid) override;
    virtual void erase_character(const lobby_character& character) override;
    virtual bool is_nickname_already_taken(const std::string& nickname) override;
    virtual lobby_character create_character(const std::string& nickname, unsigned owner_acc_id) override;
    virtual account_data get_account_data(const std::string& login) override;
};