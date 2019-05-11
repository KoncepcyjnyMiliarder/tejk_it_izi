#pragma once

#include <unordered_map>
#include <unordered_set>
#include <database_facade.hpp>
#include <friend_system_error_codes.hpp>

struct pending_friend_add_request
{
  unsigned request_sender_id;
  unsigned recipient_id;
  mutable unsigned expiration_timestamp;

  pending_friend_add_request(unsigned request_sender_id, unsigned recipient_id, unsigned expiration_timestamp)
    : request_sender_id(request_sender_id), recipient_id(recipient_id), expiration_timestamp(expiration_timestamp) {}

  struct hasher
  {
    unsigned operator()(const pending_friend_add_request& val) const
    {
      auto request_sender_id_hash = std::hash<unsigned> {}(val.request_sender_id);
      auto recipient_id_hash = std::hash<unsigned> {}(val.recipient_id);
      return (request_sender_id_hash * 0x1f1f1f1f) ^ recipient_id_hash; //stackoverflow qq
    }
  };

  struct equal
  {
    unsigned operator()(const pending_friend_add_request& lhs, const pending_friend_add_request& rhs) const
    {
      return rhs.request_sender_id == lhs.request_sender_id &&
             rhs.recipient_id == lhs.recipient_id; //dont care bout timestamp
    }
  };
};

template<typename WorldRegistrar, typename FriendlistBackend, typename RealtimeClock>
struct friend_system_base
{
  using component_backend = FriendlistBackend;
  using representative_ref = typename component_backend::representative_ref;
  using registrar = WorldRegistrar;
  using realtime_clock = RealtimeClock;

  registrar& player_registry_;
  realtime_clock& clock_;
  database_facade& db_;

  static constexpr unsigned request_expiration_time = 20000;

  friend_system_base(registrar& online_player_registry, realtime_clock& clock, database_facade& db)
    : player_registry_(online_player_registry),
      clock_(clock),
      db_(db) {}

  struct friendlist
  {
    component_backend fl_component_;
    std::unordered_set<unsigned> friend_ids_;
  };

  std::unordered_map<unsigned, friendlist> friendlist_of_id_;
  //tickable for cleanup of expired requests?
  std::unordered_set<pending_friend_add_request,
      pending_friend_add_request::hasher,
      pending_friend_add_request::equal> pending_friend_requests_;

  void register_me(representative_ref my_representative)
  {
    assert(friendlist_of_id_.count(my_representative.uid()) == 0);
    friendlist& my_friendlist = friendlist_of_id_.emplace(my_representative.uid(),
                                friendlist{ component_backend(my_representative), db_.get_friends_of(my_representative.uid()) }
                                                         ).first->second;
    component_backend& my_fl_backend = my_friendlist.fl_component_;
    std::unordered_set<unsigned>& my_friend_set = my_friendlist.friend_ids_;

    //notify all online friends
    for (auto friend_id : my_friend_set)
    {
      //check if my friend is already online
      if (friendlist_of_id_.count(friend_id))
      {
        //if so, first notify me, that he is already online
        my_fl_backend.on_friend_online_while_you_login(friendlist_of_id_.at(friend_id).fl_component_.representative().name());
        //and then notify him, that i've just logged in
        friendlist_of_id_.at(friend_id).fl_component_.on_friend_login(my_representative.name());
      }
    }
  }

  void unregister_me(representative_ref my_representative)
  {
    assert(friendlist_of_id_.count(my_representative.uid()));

    friendlist& my_friendlist = friendlist_of_id_.at(my_representative.uid());
    std::unordered_set<unsigned>& my_friend_set = my_friendlist.friend_ids_;

    //notify all online friends
    for (auto friend_id : my_friendlist.friend_ids_)
    {
      if (friendlist_of_id_.count(friend_id))
        friendlist_of_id_.at(friend_id).fl_component_.on_friend_leave(my_representative.name());
    }
    //and then unregister me
    friendlist_of_id_.erase(my_representative.uid());
  }

  void process_req_friend_add(representative_ref my_representative, const std::string& name)
  {
    assert(friendlist_of_id_.count(my_representative.uid()));

    friendlist& my_friendlist = friendlist_of_id_.at(my_representative.uid());
    component_backend& my_fl_backend = my_friendlist.fl_component_;
    std::unordered_set<unsigned>& my_friend_set = my_friendlist.friend_ids_;

    //first, fetch player's ID, only for online ones
    auto* player_ptr = player_registry_.find_online_player_by_name(name);
    //he is not online, rip
    if (player_ptr == nullptr)
    {
      my_fl_backend.on_friend_add_fail(name, friend_add_fail_reason::player_not_online);
      return;
    }
    unsigned to_add_id = player_ptr->uid();
    //you cannot add yourself, fool!
    if (to_add_id == my_representative.uid())
    {
      my_fl_backend.on_friend_add_fail(name, friend_add_fail_reason::cannot_invite_self);
      return;
    }
    //if he has been found online, it means he also has to be registered in the friendsystem - invariant!
    assert(friendlist_of_id_.count(to_add_id));
    //check, if you already are friends
    if (friendlist_of_id_.at(my_representative.uid()).friend_ids_.count(to_add_id))
    {
      my_fl_backend.on_friend_add_fail(name, friend_add_fail_reason::already_friends);
      return;
    }
    //first check, if this is response for other request
    auto iter = pending_friend_requests_.find({ to_add_id, my_representative.uid(), 0 });
    if (iter != pending_friend_requests_.end())
    {
      if (iter->expiration_timestamp < clock_.get_current_ms_stamp())
      {
        //old request expired, erase it but store this as new request (happens after leaving this if)
        pending_friend_requests_.erase(iter);
      }
      else
      {
        //we got a match!
        //inform my client
        my_fl_backend.on_new_friendship(name);
        my_friend_set.insert(to_add_id);
        //inform my friend
        friendlist_of_id_.at(to_add_id).fl_component_.on_new_friendship(my_representative.name());
        friendlist_of_id_.at(to_add_id).friend_ids_.insert(my_representative.uid());
        //erase the request as it has been processed
        pending_friend_requests_.erase(iter);
        db_.make_friends(to_add_id, my_representative.uid());
        return;
      }
    }
    //if we got here, we know that this request is NOT a response to some active previous one

    //first check, if exactly the same request has previously been sent
    auto emplace_pair = pending_friend_requests_.emplace(my_representative.uid(), to_add_id, clock_.get_current_ms_stamp() + request_expiration_time);
    //insertion took place, request wasnt here before
    if (emplace_pair.second)
    {
      friendlist_of_id_.at(to_add_id).fl_component_.on_friendship_request(my_representative.name());
      my_fl_backend.on_my_request_sent(my_representative.name(), friend_add_request_result::sent_succesfully);
      return;
    }
    //insertion didnt take place, check last timestamp
    if (emplace_pair.first->expiration_timestamp < clock_.get_current_ms_stamp())
    {
      //old request expired, let's just refresh the timestamp and renotify the recipient
      emplace_pair.first->expiration_timestamp = clock_.get_current_ms_stamp() + request_expiration_time;
      friendlist_of_id_.at(to_add_id).fl_component_.on_friendship_request(my_representative.name());
      my_fl_backend.on_my_request_sent(my_representative.name(), friend_add_request_result::sent_succesfully);
      return;
    }
    //well well welly my friend... the old request didnt expire yet, chill the fuck out!
    my_fl_backend.on_my_request_sent(name, friend_add_request_result::previous_still_waiting);
  }

  void process_req_friend_remove(representative_ref my_representative, const std::string& name)
  {
    assert(friendlist_of_id_.count(my_representative.uid()));

    friendlist& my_friendlist = friendlist_of_id_.at(my_representative.uid());
    component_backend& my_fl_backend = my_friendlist.fl_component_;
    std::unordered_set<unsigned>& my_friend_set = my_friendlist.friend_ids_;

    //fetch his ID
    unsigned to_remove_id = player_registry_.find_player_id_by_name_in_database(name);
    //does he even exist?
    if (to_remove_id == registrar::invalid_id)
    {
      my_fl_backend.on_friend_removal_fail(name);
      return;
    }
    //check if he is my friend in first place
    if (my_friend_set.count(to_remove_id) == 0)
    {
      my_fl_backend.on_friend_removal_fail(name);
      return;
    }
    //he IS my friend, but not for long anymore, tipesh!
    my_fl_backend.on_broken_friendship(name);
    my_friend_set.erase(to_remove_id);
    //if is online...
    if (friendlist_of_id_.count(to_remove_id))
    {
      friendlist_of_id_.at(to_remove_id).fl_component_.on_broken_friendship(my_representative.name());
      friendlist_of_id_.at(to_remove_id).friend_ids_.erase(my_representative.uid());
    }
    db_.remove_friends(to_remove_id, my_representative.uid());
  }
};
