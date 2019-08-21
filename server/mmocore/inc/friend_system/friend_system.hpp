#pragma once

#include <database_facade.hpp>
#include <world_registrar.hpp>
#include <tii_entity_representative.hpp>
#include <friend_system/friend_backend.hpp>
#include <friend_system/pending_friend_requests.hpp>

//very temp clock impl
#include <chrono>
struct realtime_clock
{
  std::chrono::system_clock::time_point get_current_time()
  {
    return std::chrono::system_clock::now();
  }
};

class friend_system
{
    world_registrar& player_registry_;
    realtime_clock& clock_;
    database_facade& db_;
    pending_friend_requests pending_requests_;

    struct friendlist
    {
      std::unique_ptr<friend_backend> backend;
      std::unordered_set<unsigned> friend_ids;
    };

    std::unordered_map<unsigned, friendlist> friendlist_of_id_;

  public:

    //RETHINK tii_entity_representative maybe - tii_entity?
    friend_system(world_registrar& online_player_registry, realtime_clock& clock, database_facade& db, task_scheduler& scheduler);

    void register_me(tii_entity_representative::const_reference my_representative, std::unique_ptr<friend_backend> my_backend);
    void unregister_me(tii_entity_representative::const_reference my_representative);
    void process_req_friend_add(tii_entity_representative::const_reference my_representative, const std::string& name);
    void process_req_friend_remove(tii_entity_representative::const_reference my_representative, const std::string& name);
};
