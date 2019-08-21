#pragma once

#include <unordered_map>
#include <tii_entity_representative.hpp>
#include <database_facade.hpp>
#include <cassert>

class world_registrar
{

    //redundancy for quicker search
    std::unordered_map<unsigned, tii_entity_representative::const_pointer> id_to_character_;
    std::unordered_map<std::string, tii_entity_representative::const_pointer> name_to_character_;

    database_facade& db_;

  public:

    world_registrar(database_facade& db)
      : db_(db) {}

    //qq, wielkieqq, or even bigger qq
    static constexpr unsigned invalid_id = 0;

    void register_me(tii_entity_representative::const_reference character)
    {
      assert(id_to_character_.count(character.uid()) == 0);
      assert(name_to_character_.count(character.name()) == 0);
      id_to_character_.emplace(character.uid(), &character);
      name_to_character_.emplace(character.name(), &character);
    }

    void unregister_me(tii_entity_representative::const_reference character)
    {
      assert(id_to_character_.count(character.uid()));
      assert(name_to_character_.count(character.name()));
      id_to_character_.erase(character.uid());
      name_to_character_.erase(character.name());
    }

    tii_entity_representative::const_pointer find_online_player_by_name(const std::string& name)
    {
      auto it = name_to_character_.find(name);
      if (it == name_to_character_.end())
        return nullptr;
      return name_to_character_.at(name);
    }

    tii_entity_representative::const_pointer find_online_player_by_id(unsigned id)
    {
      auto it = id_to_character_.find(id);
      if (it == id_to_character_.end())
        return nullptr;
      return it->second;
    }

    unsigned find_player_id_by_name_in_database(const std::string& name)
    {
      return db_.char_name_to_uid(name);
    }
};