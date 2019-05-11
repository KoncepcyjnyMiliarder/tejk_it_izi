#pragma once

#include <unordered_map>
#include <database_facade.hpp>
#include <cassert>

template<typename Character>
class world_registrar
{
  public:

    using character_ptr = typename Character::pointer;
    using character_const_ptr = typename Character::const_pointer;
    using character_ref = typename Character::reference;
    using character_const_ref = typename Character::const_reference;

  protected:

    //redundancy for quicker search
    std::unordered_map<unsigned, character_ptr> id_to_character_;
    std::unordered_map<std::string, character_ptr> name_to_character_;

  private:

    database_facade& db_;

  public:

    world_registrar(database_facade& db)
      : db_(db) {}

    //qq, wielkieqq, or even bigger qq
    static constexpr unsigned invalid_id = 0;

    void register_me(character_ref character)
    {
      assert(id_to_character_.count(character.uid()) == 0);
      assert(name_to_character_.count(character.name()) == 0);
      id_to_character_.emplace(character.uid(), &character);
      name_to_character_.emplace(character.name(), &character);
      //notify everyone around
    }

    void unregister_me(character_ref character)
    {
      assert(id_to_character_.count(character.uid()));
      assert(name_to_character_.count(character.name()));
      id_to_character_.erase(character.uid());
      name_to_character_.erase(character.name());
      //notify everyone around
    }

    character_const_ptr find_online_player_by_name(const std::string& name)
    {
      auto it = name_to_character_.find(name);
      if (it == name_to_character_.end())
        return nullptr;
      return name_to_character_.at(name);
    }

    unsigned find_player_id_by_name_in_database(const std::string& name)
    {
      return db_.char_name_to_uid(name);
    }
};