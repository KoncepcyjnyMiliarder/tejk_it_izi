#include <ingame_state_recv_packet_factory.hpp>
#include <binary_deserializer.hpp>
#include <null_received_packet.hpp>
#include <c2s/join_chat.hpp>
#include <c2s/leave_chat.hpp>
#include <c2s/log_off_to_lobby.hpp>
#include <c2s/send_chat_msg.hpp>
#include <ingame_state_protocol.hpp>

ingame_state_recv_packet_factory::ingame_state_recv_packet_factory(std::shared_ptr<net_session> my_session,
    state_transitioner& transitioner, world& universe, logger& logger, database_facade& db, world::player_pawn& my_pawn,
    account_data& acc_data, std::list<std::unique_ptr<world::chat_system::chat_pressence_token>>& chatrooms_im_in)
  : my_session_(my_session),
    transitioner_(transitioner),
    world_(universe),
    logger_(logger),
    db_(db),
    my_pawn_(my_pawn),
    acc_data_(acc_data),
    chatrooms_im_in_(chatrooms_im_in)
{
}

received_packet::packet_ptr ingame_state_recv_packet_factory::construct(const std::array<char, 2048>& data, unsigned size)
{
  binary_deserializer bd(data.data(), size);
  ingame_state_protocol::to_server_packet_opcodes opcode;
  bd >> opcode;
  switch (opcode)
  {
    case ingame_state_protocol::join_chat:
      return std::make_unique<join_chat>(bd, my_session_, logger_, my_pawn_, world_.chat_, chatrooms_im_in_);
    case ingame_state_protocol::leave_chat:
      return std::make_unique<leave_chat>(bd, logger_, chatrooms_im_in_);
    case ingame_state_protocol::log_off_to_lobby:
      return std::make_unique<log_off_to_lobby>(my_session_, logger_, transitioner_, world_, db_, acc_data_);
    case ingame_state_protocol::send_chat_msg:
      return std::make_unique<send_chat_msg>(bd, my_session_, logger_, world_.chat_, chatrooms_im_in_);
  }
  return std::make_unique<null_received_packet>();
}
