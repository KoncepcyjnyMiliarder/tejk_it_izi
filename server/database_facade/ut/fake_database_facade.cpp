#include <fake_database_facade.hpp>
#include <algorithm>

fake_database_facade::fake_database_facade()
  : next_char_id_serial_(7)
{
  acc_datas_["krzysztof"] = { 1, "krzysztof", "asdas", "53a666af08d5389b88ab", 9999 };
  acc_datas_["mirek"] = { 2, "mirek", "krojcburg", "b099c55d001e4e14775c", 99 };
  acc_datas_["foo"] = { 3, "foo", "bar", "6f054bb62c30c23f5ee8", 0 };

  account_to_chars_[1].emplace_back(1, "Krzysztoforek");
  account_to_chars_[1].emplace_back(2, "KoncepcyjnyMiliarder");
  account_to_chars_[2].emplace_back(3, "Mirraseq");
  account_to_chars_[2].emplace_back(4, "Tag");
  account_to_chars_[2].emplace_back(5, "JohnFreeman");
  account_to_chars_[3].emplace_back(6, "foobar");

  account_to_friends_[1] = { 3, 4, 6 };
  account_to_friends_[2] = { 4 };
  account_to_friends_[3] = { 1 };
  account_to_friends_[4] = { 1, 2 };
  account_to_friends_[6] = { 1 };
}

lobby_character::lobby_character_list fake_database_facade::get_lobby_chars(unsigned account_uid)
{
  return account_to_chars_[account_uid];
}

void fake_database_facade::erase_character(const lobby_character& character)
{
  for (auto& map_node : account_to_chars_)
  {
    auto& char_list = map_node.second;
    char_list.remove_if([&character](const lobby_character & elem)
    {
      return elem.name == character.name;
    });
  }
}

bool fake_database_facade::is_nickname_already_taken(const std::string& nickname)
{
  for (auto& map_node : account_to_chars_)
  {
    auto& char_list = map_node.second;
    if(std::any_of(char_list.begin(), char_list.end(), [&nickname](const lobby_character & elem)
  {
    return elem.name == nickname;
  }))
    return true;
  }
  return false;
}

lobby_character fake_database_facade::create_character(const std::string& nickname, unsigned owner_acc_id)
{
  account_to_chars_[owner_acc_id].emplace_back(next_char_id_serial_++, nickname);
  return account_to_chars_[owner_acc_id].back();
}

account_data fake_database_facade::get_account_data(const std::string& login)
{
  return acc_datas_.at(login);
}

std::unordered_set<unsigned> fake_database_facade::get_friends_of(unsigned char_id)
{
  if (account_to_friends_.count(char_id))
    return account_to_friends_.at(char_id);
  return {};
}

void fake_database_facade::make_friends(unsigned one, unsigned other)
{
  account_to_friends_[one].insert(other);
  account_to_friends_[other].insert(one);
}

void fake_database_facade::remove_friends(unsigned one, unsigned other)
{
  account_to_friends_.at(one).erase(other);
  account_to_friends_.at(other).erase(one);
}
