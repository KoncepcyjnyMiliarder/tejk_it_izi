#pragma once

#include <world.hpp>

class friends_controller
{

  public:

    using representative_ref = typename tii_player_character::reference;

  private:

    representative_ref my_representative_;
    world::friend_system& fs_;

  public:

    friends_controller(representative_ref& my_representative, world::friend_system& fs);
    ~friends_controller();

    void on_req_friend_add(const std::string& name);
    //void on_req_deny_friendship_req(const std::string& name);
    void on_req_friend_remove(const std::string& name);
};
