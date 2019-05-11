#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <list>
#include <algorithm>
#include <cassert>

template<typename ParticipantT>
struct chat_system_base
{

  using participant_ptr = typename ParticipantT::pointer;
  using participant_reference = typename ParticipantT::reference;

  struct chatroom
  {
    using pointer = chatroom*;

    const std::string room_name;
    std::list<participant_ptr> participants;
    chat_system_base& the_system;

    chatroom(const std::string& name, chat_system_base& the_system)
      : room_name(name), the_system(the_system) {}

    void broadcast_message(const std::string& msg, participant_reference sender)
    {
      for (auto& participant : participants)
        participant->on_chat_message(room_name, sender.user_name(), msg);
    }

    void leave(participant_reference user)
    {
      for (auto& elem : participants)
        elem->on_user_left_chat(room_name, user.user_name());
      participants.remove(&user);
      if (participants.size() == 0)
        the_system.unregister_chatserver(room_name);
    }
  };

  std::unordered_map<std::string, chatroom> active_chatrooms;

  using chatroom_ptr = typename chatroom::pointer;

  void unregister_chatserver(const std::string& name)
  {
    assert(active_chatrooms.count(name) != 0);
    assert(active_chatrooms.at(name).participants.empty());
    active_chatrooms.erase(name);
  }

  chatroom_ptr register_participant(const std::string& channel_name, participant_reference participant)
  {
    chatroom& selected_channel = active_chatrooms.emplace(channel_name, chatroom(channel_name, *this)).first->second;
    assert(std::find(selected_channel.participants.begin(), selected_channel.participants.end(), &participant) == selected_channel.participants.end());
    selected_channel.participants.push_back(&participant);
    for (auto& elem : selected_channel.participants)
      elem->on_user_joined_chat(channel_name, participant.user_name());
    return &selected_channel;
  }
};
