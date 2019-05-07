#include <c2s/leave_chat.hpp>
#include <s2c/user_left_chat.hpp>
#include <algorithm>

leave_chat::leave_chat(binary_deserializer& bd, logger& logger,
                       std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in)
  :  logger_(logger),
     chatrooms_im_in_(chatrooms_im_in)
{
  bd >> chatroom_name_;
}

void leave_chat::execute_associated_action()
{
  auto the_chatroom = std::find_if(chatrooms_im_in_.begin(), chatrooms_im_in_.end(), [this](const auto & elem)
  {
    return elem->room->room_name == chatroom_name_;
  });

  if (the_chatroom == chatrooms_im_in_.end())
    return;

  chatrooms_im_in_.erase(the_chatroom);
}
