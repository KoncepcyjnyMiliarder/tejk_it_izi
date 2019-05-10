#include <psql_facade.hpp>
#include <stdexcept>

psql_facade::psql_facade(const std::string& username, const std::string& pass, const std::string& dbname)
  : database_(username, pass, dbname)
{
}

lobby_character::lobby_character_list psql_facade::get_lobby_chars(unsigned account_uid)
{
  lobby_character::lobby_character_list res;
  pqxx::work w(database_.get_conn());
  pqxx::result query_res(w.exec("SELECT id, nickname FROM characters WHERE owner_account_id = " + std::to_string(account_uid)));
  for (const auto& row : query_res)
    res.emplace_back(row[0].as(unsigned()), row[1].as(std::string()));
  return res;
}

void psql_facade::erase_character(const lobby_character& character)
{
  pqxx::work w(database_.get_conn());
  pqxx::result query_res(w.exec("DELETE FROM characters WHERE nickname = \'" + character.name + '\''));
  w.commit();
}

bool psql_facade::is_nickname_already_taken(const std::string& nickname)
{
  pqxx::work w(database_.get_conn());
  pqxx::result query_res(w.exec("SELECT id FROM characters WHERE nickname = \'" + nickname + '\''));
  return !query_res.empty();
}

lobby_character psql_facade::create_character(const std::string& nickname, unsigned owner_acc_id)
{
  pqxx::work w(database_.get_conn());
  pqxx::result query_res(
    w.exec("INSERT INTO characters(owner_account_id, nickname) VALUES (" + std::to_string(owner_acc_id) +
           ", \'" + nickname + "\') RETURNING id"));
  if (query_res.empty())
    throw std::runtime_error("Character creation query failed");
  unsigned uid;
  query_res[0][0].to(uid);
  w.commit();
  return lobby_character(uid, nickname);
}

account_data psql_facade::get_account_data(const std::string& login)
{
  account_data res;

  pqxx::work w(database_.get_conn());
  pqxx::result details_query(w.exec("SELECT id, pass_hash, pass_salt FROM accounts WHERE login = \'" + login + '\''));
  if (details_query.empty())
    throw std::runtime_error("No database entry for login " + login);
  details_query[0][0].to(res.uid);
  details_query[0][1].to(res.pass_hash);
  details_query[0][2].to(res.salt);
  res.login = login;
  pqxx::result access_query(w.exec("SELECT privilege_level FROM access_level WHERE account_id = " + std::to_string(res.uid)));
  access_query.empty() ? res.access_level = 0 : access_query[0][0].to(res.access_level);
  return res;
}

std::unordered_set<unsigned> psql_facade::get_friends_of(unsigned char_id)
{
  pqxx::work w(database_.get_conn());
  pqxx::result query_res(w.exec("SELECT character_id as id FROM character_friends WHERE his_friend_id = " + std::to_string(char_id) + " UNION "
                                "SELECT his_friend_id as id FROM character_friends WHERE character_id = " + std::to_string(char_id)));
  std::unordered_set<unsigned> res;
  for (const auto& row : query_res)
    res.emplace(row[0].as(unsigned()));
  return res;
}

void psql_facade::make_friends(unsigned one, unsigned other)
{
  if (one > other)
    std::swap(one, other);
  pqxx::work w(database_.get_conn());
  pqxx::result query_res(w.exec("INSERT INTO character_friends(character_id, his_friend_id) VALUES (" + std::to_string(one) +
                                ", " + std::to_string(other) + ")"));
  w.commit();
}

void psql_facade::remove_friends(unsigned one, unsigned other)
{
  if (one > other)
    std::swap(one, other);
  pqxx::work w(database_.get_conn());
  pqxx::result query_res(w.exec("DELETE FROM character_friends WHERE character_id = " + std::to_string(one) +
                                " AND his_friend_id = " + std::to_string(other)));
  w.commit();
}
