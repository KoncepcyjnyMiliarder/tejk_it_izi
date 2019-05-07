#include <map>
#include <memory>
#include <string>
#include <list>

//let's fucking template the livin shit out of this feature

template<typename ParticipantT>
struct chatroom
{
  using chatroom_ptr = std::shared_ptr<chatroom<ParticipantT>>;
  using participant_ptr = typename ParticipantT::pointer;

  chatroom(const std::string& name)
    : room_name(name) {}

  std::string room_name;
  std::list<participant_ptr> participants;
};

template<typename ParticipantT>
struct chat_subsystem
{
  using chatroom_t = chatroom<ParticipantT>;
  using chatroom_ptr = typename chatroom_t::chatroom_ptr;
  using participant_ptr = typename chatroom_t::participant_ptr;

  std::map<std::string, chatroom_ptr> active_chatrooms;

  struct chat_pressence_token
  {
    chat_subsystem& big_boss_XD;
    chatroom_ptr room;
    participant_ptr user;

    chat_pressence_token(chat_subsystem& cs, chatroom_ptr room, participant_ptr user)
      : big_boss_XD(cs), room(room), user(user) {}

    ~chat_pressence_token()
    {
      for (auto& elem : room->participants)
        elem->on_user_left_chat(room->room_name, user->user_name());
      room->participants.remove(user);
      if (room->participants.size() == 0)
        big_boss_XD.unregister_chatserver(room->room_name);
    }
  };

  //a to prywatne tylko dla tonkena na unregister
  void unregister_chatserver(const std::string& name)
  {
    active_chatrooms.erase(name);
  }

  std::unique_ptr<chat_pressence_token> register_participant(const std::string& channel_name, participant_ptr participant)
  {
    //does channel exist?
    if (active_chatrooms.count(channel_name) == 0)
      active_chatrooms.emplace(channel_name, std::make_shared<chatroom_t>(channel_name));
    //now it definitely does
    chatroom_ptr& selected_channel = active_chatrooms.at(channel_name);
    //check if he is laready in the chat or allow multiple pressence lol?
    //whatever for now
    selected_channel->participants.push_back(participant);
    for (auto& elem : selected_channel->participants)
      elem->on_user_joined_chat(channel_name, participant->user_name());
    return std::make_unique<chat_pressence_token>(*this, selected_channel, participant);
  }

  static void broadcast_message(const chat_pressence_token& token, const std::string& msg)
  {
    for (auto& participant : token.room->participants)
      participant->on_chat_message(token.room->room_name, token.user->user_name(), msg);
  }
};
