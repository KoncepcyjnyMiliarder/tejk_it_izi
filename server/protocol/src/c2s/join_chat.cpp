#include <c2s/join_chat.hpp>
#include <s2c/user_joined_chat.hpp>

join_chat::join_chat(binary_deserializer& bd, std::shared_ptr<net_session> my_session, logger& logger,
                     world::player_pawn& my_pawn, world::chat_system& chat_sys,
                     std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in)
  : my_session_(my_session),
    logger_(logger),
    my_pawn_(my_pawn),
    chat_sys_(chat_sys),
    chatrooms_im_in_(chatrooms_im_in)
{
  bd >> chatroom_name_;
}

void join_chat::execute_associated_action()
{
  chatrooms_im_in_.emplace_back(chat_sys_.register_participant(chatroom_name_, &my_pawn_));
}
