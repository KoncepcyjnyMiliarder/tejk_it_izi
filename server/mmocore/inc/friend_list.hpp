#pragma once

#include <unordered_set>
#include <database_facade.hpp>
#include <net_session.hpp>

class friend_list
{
    std::shared_ptr<net_session> my_session_;

    std::unordered_set<unsigned> my_bidirectional_friend_ids_;
    std::unordered_set<unsigned> one_dir_friends_of_me_;
    std::unordered_set<unsigned> one_dir_others_having_me_;

  public:

    using pointer = friend_list*;

    friend_list(database_facade& db, std::shared_ptr<net_session> my_session);

    void on_you_have_been_added(unsigned by_char_id);
    void on_you_have_been_removed(unsigned by_char_id);
};