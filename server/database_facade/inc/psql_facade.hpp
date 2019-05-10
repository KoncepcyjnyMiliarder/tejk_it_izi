#pragma once

#include <database_facade.hpp>
#include <postgresql_db.hpp>

class psql_facade
  : public database_facade
{
    postgresql_db database_;

  public:

    psql_facade(const std::string& username, const std::string& pass, const std::string& dbname);

    virtual lobby_character::lobby_character_list get_lobby_chars(unsigned account_uid) override;
    virtual void erase_character(const lobby_character& character)  override;
    virtual bool is_nickname_already_taken(const std::string& nickname)  override;
    virtual lobby_character create_character(const std::string& nickname, unsigned owner_acc_id) override;
    virtual account_data get_account_data(const std::string& login) override;
    virtual std::unordered_set<unsigned> get_friends_of(unsigned char_id) override;
    virtual void make_friends(unsigned one, unsigned other) override;
    virtual void remove_friends(unsigned one, unsigned other) override;
};
