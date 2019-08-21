#include <friend_system/friend_system.hpp>

friend_system::friend_system(world_registrar& online_player_registry, realtime_clock& clock, database_facade& db, task_scheduler& scheduler)
  : player_registry_(online_player_registry),
    clock_(clock),
    db_(db),
    pending_requests_(scheduler)
{
}

void friend_system::register_me(tii_entity_representative::const_reference my_representative, std::unique_ptr<friend_backend> my_backend)
{
  assert(friendlist_of_id_.count(my_representative.uid()) == 0);
  friendlist& my_friendlist = friendlist_of_id_.emplace(my_representative.uid(),
                              friendlist{ std::move(my_backend), db_.get_friends_of(my_representative.uid()) }
                                                       ).first->second;
  //notify all online friends
  for (auto friend_id : my_friendlist.friend_ids)
  {
    //check if my friend is already online
    if (friendlist_of_id_.count(friend_id))
    {
      //if so, first notify me, that he is already online
      auto friend_entity_ptr = player_registry_.find_online_player_by_id(friend_id);
      assert(friend_entity_ptr);
      my_friendlist.backend->on_friend_online_while_you_login(friend_entity_ptr->name());
      //and then notify him, that i've just logged in
      friendlist_of_id_.at(friend_id).backend->on_friend_login(my_representative.name());
    }
  }
}

void friend_system::unregister_me(tii_entity_representative::const_reference my_representative)
{
  assert(friendlist_of_id_.count(my_representative.uid()));

  //notify all online friends
  for (auto friend_id : friendlist_of_id_.at(my_representative.uid()).friend_ids)
  {
    if (friendlist_of_id_.count(friend_id))
      friendlist_of_id_.at(friend_id).backend->on_friend_leave(my_representative.name());
  }
  //and then unregister me
  friendlist_of_id_.erase(my_representative.uid());
}

void friend_system::process_req_friend_add(tii_entity_representative::const_reference my_representative, const std::string& name)
{
  assert(friendlist_of_id_.count(my_representative.uid()));

  friendlist& my_friendlist = friendlist_of_id_.at(my_representative.uid());
  auto& my_backend = my_friendlist.backend;
  std::unordered_set<unsigned>& my_friend_set = my_friendlist.friend_ids;

  //first, fetch player's ID, only for online ones
  auto player_ptr = player_registry_.find_online_player_by_name(name);
  //he is not online, rip
  if (player_ptr == nullptr)
  {
    my_backend->on_friend_add_fail(name, friend_add_fail_reason::player_not_online);
    return;
  }
  unsigned to_add_id = player_ptr->uid();
  //you cannot add yourself, fool!
  if (to_add_id == my_representative.uid())
  {
    my_backend->on_friend_add_fail(name, friend_add_fail_reason::cannot_invite_self);
    return;
  }
  //if he has been found online, it means he also has to be registered in the friendsystem - invariant!
  assert(friendlist_of_id_.count(to_add_id));
  //check, if you already are friends
  if (my_friend_set.count(to_add_id))
  {
    my_backend->on_friend_add_fail(name, friend_add_fail_reason::already_friends);
    return;
  }

  //make use of the pending request holder
  auto request_state = pending_requests_.process_add_friend_request(my_representative.uid(), to_add_id, clock_.get_current_time());
  switch (request_state)
  {
    case pending_friend_requests::operation_result::already_in_buffer:
      //well well welly my friend... the old request didnt expire yet, chill the fuck out!
      my_backend->on_my_request_sent(name, friend_add_request_result::previous_still_waiting);
      break;
    case pending_friend_requests::operation_result::succesfully_added:
      friendlist_of_id_.at(to_add_id).backend->on_friendship_request(my_representative.name());
      my_backend->on_my_request_sent(name, friend_add_request_result::sent_succesfully);
      break;
    case pending_friend_requests::operation_result::that_was_a_match:
      //inform me
      my_backend->on_new_friendship(name);
      my_friend_set.insert(to_add_id);
      //inform my friend
      friendlist_of_id_.at(to_add_id).backend->on_new_friendship(my_representative.name());
      friendlist_of_id_.at(to_add_id).friend_ids.insert(my_representative.uid());
      db_.make_friends(to_add_id, my_representative.uid());
      break;
    default:
      assert(false);
  }
}

void friend_system::process_req_friend_remove(tii_entity_representative::const_reference my_representative, const std::string& name)
{
  assert(friendlist_of_id_.count(my_representative.uid()));

  friendlist& my_friendlist = friendlist_of_id_.at(my_representative.uid());
  auto& my_backend = my_friendlist.backend;
  std::unordered_set<unsigned>& my_friend_set = my_friendlist.friend_ids;

  //fetch his ID
  unsigned to_remove_id = player_registry_.find_player_id_by_name_in_database(name);
  //does he even exist?
  if (to_remove_id == world_registrar::invalid_id)
  {
    my_backend->on_friend_removal_fail(name);
    return;
  }
  //check if he is my friend in first place
  if (my_friend_set.count(to_remove_id) == 0)
  {
    my_backend->on_friend_removal_fail(name);
    return;
  }
  //he IS my friend, but not for long anymore, tipesh!
  my_backend->on_broken_friendship(name);
  my_friend_set.erase(to_remove_id);
  //if is online...
  if (friendlist_of_id_.count(to_remove_id))
  {
    friendlist_of_id_.at(to_remove_id).backend->on_broken_friendship(my_representative.name());
    friendlist_of_id_.at(to_remove_id).friend_ids.erase(my_representative.uid());
  }
  db_.remove_friends(to_remove_id, my_representative.uid());
}
