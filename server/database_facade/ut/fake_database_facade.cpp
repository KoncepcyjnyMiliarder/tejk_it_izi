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
