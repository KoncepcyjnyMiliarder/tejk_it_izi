#include <chat_controller.hpp>
#include <algorithm>
#include <algorithm>

chat_controller::chat_controller(chat_system_base<chat_participant>& chat_sys,
                                 std::shared_ptr<net_session> my_session,
                                 const std::string& my_name)
  : chat_sys_(chat_sys),
    my_representative_(my_session, my_name)
{
}

chat_controller::~chat_controller()
{
  for (auto& chatroom : chatrooms_im_in_)
    chatroom->leave(my_representative_);
}

void chat_controller::register_me(const std::string& chatroom_name)
{
  chatrooms_im_in_.push_back(chat_sys_.register_participant(chatroom_name, my_representative_));
}

void chat_controller::unregister_me(const std::string& chatroom_name)
{
  auto the_chatroom = std::find_if(chatrooms_im_in_.begin(), chatrooms_im_in_.end(), [&chatroom_name](const auto & elem)
  {
    return elem->room_name == chatroom_name;
  });
  if (the_chatroom == chatrooms_im_in_.end())
    return;
  chat_system_base<chat_participant>::chatroom_ptr chatroom = *the_chatroom;
  chatroom->leave(my_representative_);
  chatrooms_im_in_.erase(the_chatroom);
}

void chat_controller::broadcast_message(const std::string& chatroom_name, const std::string& msg)
{
  auto the_chatroom = std::find_if(chatrooms_im_in_.begin(), chatrooms_im_in_.end(), [&chatroom_name](const auto & elem)
  {
    return elem->room_name == chatroom_name;
  });

  if (the_chatroom == chatrooms_im_in_.end())
    return;
  chat_system_base<chat_participant>::chatroom_ptr chatroom = *the_chatroom;
  chatroom->broadcast_message(msg, my_representative_);
}

std::size_t chat_controller::count_of_chatrooms_im_in() const
{
  return chatrooms_im_in_.size();
}
