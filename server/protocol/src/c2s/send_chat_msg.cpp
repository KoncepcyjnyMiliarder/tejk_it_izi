#include <c2s/send_chat_msg.hpp>
#include <s2c/user_joined_chat.hpp>
#include <algorithm>

send_chat_msg::send_chat_msg(binary_deserializer& bd, std::shared_ptr<net_session> my_session, logger& logger,
                             world::chat_system& chat_sys,
                             std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in)
  : my_session_(my_session),
    logger_(logger),
    chat_sys_(chat_sys),
    chatrooms_im_in_(chatrooms_im_in)
{
  bd >> chatroom_name_ >> message_;
}

void send_chat_msg::execute_associated_action()
{
  auto the_chatroom = std::find_if(chatrooms_im_in_.begin(), chatrooms_im_in_.end(), [this](const auto & elem)
  {
    return elem->room->room_name == chatroom_name_;
  });

  if (the_chatroom == chatrooms_im_in_.end())
    return;

  chat_sys_.broadcast_message(**the_chatroom, message_);
}
