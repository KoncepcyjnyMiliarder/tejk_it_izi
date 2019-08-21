#include <chat_system/chat_system.hpp>
#include <cassert>

void chat_system::unregister_from_chatroom(tii_entity_representative::const_pointer my_representative, chatroom* room)
{
  //this only erases me from chatroom map, i got to erase the chatroom from my own set on my own
  assert(room);
  assert(active_chatrooms_.count(room->room_name));
  assert(room->participants.count(my_representative));
  assert(entities_.at(my_representative).chatrooms_im_in.count(room));
  room->participants.erase(my_representative);
  if (room->participants.size() == 0)
  {
    active_chatrooms_.erase(room->room_name);
    return;
  }
  for (auto& participant : room->participants)
    entities_.at(participant).backend->on_user_left_chat(room->room_name, my_representative->name());
}

void chat_system::periodic_broadcast()
{
  for (auto& chatroom_pair : active_chatrooms_)
  {
    chatroom& selected_channel = chatroom_pair.second;
    const std::string sender_name("[[BROADCAST - " + selected_channel.room_name + "]]");
    const std::string message("There are currently: " + std::to_string(selected_channel.participants.size()) + " users on the chat!");
    for (auto& participant : selected_channel.participants)
      entities_.at(participant).backend->on_chat_message(selected_channel.room_name, sender_name, message);
  }
  periodic_broadcast_task_ = std::move(scheduler_.postpone_task(std::bind(&chat_system::periodic_broadcast, this), boost::posix_time::seconds(6)));
}

chat_system::chat_system(task_scheduler& scheduler, world_registrar& online_player_registry)
  : scheduler_(scheduler), online_player_registry_(online_player_registry)
{
  periodic_broadcast_task_ = std::move(scheduler_.postpone_task(std::bind(&chat_system::periodic_broadcast, this), boost::posix_time::seconds(6)));
}

void chat_system::register_me(tii_entity_representative::const_reference my_representative, std::unique_ptr<chat_backend> my_backend)
{
  assert(entities_.count(&my_representative) == 0);
  entities_.emplace(&my_representative, entity_data{ std::move(my_backend) });
}

void chat_system::unregister_me(tii_entity_representative::const_reference my_representative)
{
  assert(entities_.count(&my_representative));
  //unregister from all rooms
  for (const auto& room : entities_.at(&my_representative).chatrooms_im_in)
    unregister_from_chatroom(&my_representative, room);
  //erase from system
  entities_.erase(&my_representative);
}

void chat_system::join_chatroom(tii_entity_representative::const_reference my_representative, const std::string& room_name)
{
  assert(entities_.count(&my_representative));
  //check, if room exists or not
  auto iter = active_chatrooms_.find(room_name);
  if (iter == active_chatrooms_.end())
    iter = active_chatrooms_.emplace(room_name, room_name).first;
  //am i already in it?
  chatroom* room = &iter->second;
  if (room->participants.count(&my_representative))
    return;
  //well then, register!
  room->participants.insert(&my_representative);

  for (auto& participant : room->participants)
    entities_.at(participant).backend->on_user_joined_chat(room_name, my_representative.name());

  assert(entities_.at(&my_representative).chatrooms_im_in.count(room) == 0);
  entities_.at(&my_representative).chatrooms_im_in.insert(room);
}

void chat_system::leave_chatroom(tii_entity_representative::const_reference my_representative, const std::string& room_name)
{
  assert(entities_.count(&my_representative));
  //does the room even exist?
  auto iter = active_chatrooms_.find(room_name);
  if (iter == active_chatrooms_.end())
    return;
  //am i in the room?
  chatroom* room = &iter->second;
  if (room->participants.count(&my_representative) == 0)
    return;
  //yes, i am!
  unregister_from_chatroom(&my_representative, &iter->second);
  assert(entities_.at(&my_representative).chatrooms_im_in.count(room));
  entities_.at(&my_representative).chatrooms_im_in.erase(room);
}

void chat_system::broadcast_message(tii_entity_representative::const_reference my_representative, const std::string& room_name, const std::string& msg)
{
  assert(entities_.count(&my_representative));
  //similar as above
  auto iter = active_chatrooms_.find(room_name);
  if (iter == active_chatrooms_.end())
    return;
  chatroom* room = &iter->second;
  if (room->participants.count(&my_representative) == 0)
    return;
  assert(entities_.at(&my_representative).chatrooms_im_in.count(room));
  for (auto& participant : room->participants)
    entities_.at(participant).backend->on_chat_message(room->room_name, my_representative.name(), msg);
}

void chat_system::whisper(tii_entity_representative::const_reference my_representative, const std::string& recipient, const std::string& msg)
{
  assert(entities_.count(&my_representative));
  //find the recipient
  auto his_entity_ptr = online_player_registry_.find_online_player_by_name(recipient);
  if (his_entity_ptr == nullptr || his_entity_ptr == &my_representative)
    return;
  assert(entities_.count(his_entity_ptr));
  entities_.at(his_entity_ptr).backend->on_whisper(my_representative.name(), msg);
}
