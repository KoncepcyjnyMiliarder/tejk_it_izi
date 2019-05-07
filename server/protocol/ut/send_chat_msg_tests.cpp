#include <gtest/gtest.h>
#include <binary_serializer.hpp>
#include <c2s/join_chat.hpp>
#include <c2s/send_chat_msg.hpp>
#include <fake_net_session.hpp>
#include <fake_net_socket.hpp>
#include <fake_logger.hpp>
#include <ingame_state_protocol.hpp>

TEST(send_chat_msg, construction)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name") << std::string("some_message");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  world universe;
  world::player_pawn my_pawn(universe, session, "Krzysztof");
  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in;
  EXPECT_NO_THROW(send_chat_msg(incoming_stream, session, log, universe.chat_, chatrooms_im_in));
}

TEST(send_chat_msg, throw_on_packet_underflow)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name"); // didnt send message
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  world universe;
  world::player_pawn my_pawn(universe, session, "Krzysztof");
  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in;
  EXPECT_THROW(send_chat_msg(incoming_stream, session, log, universe.chat_, chatrooms_im_in), std::underflow_error);
}

TEST(send_chat_msg, send_message_to_all_participants)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("czekoladowa wysepka") << std::string("no eloszki pysiaczki");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  world universe;
  world::player_pawn my_pawn(universe, session, "Krzysztof");
  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in;
  auto other_guys_socket = std::make_shared<fake_net_socket>();
  auto other_guys_session(std::make_shared<fake_net_session>(other_guys_socket));
  world::player_pawn other_pawn(universe, other_guys_session, "Someone_else");
  auto other_guy = universe.chat_.register_participant("czekoladowa wysepka", &other_pawn);
  chatrooms_im_in.push_back(universe.chat_.register_participant("czekoladowa wysepka", &my_pawn));
  send_chat_msg(incoming_stream, session, log, universe.chat_, chatrooms_im_in).execute_associated_action();
  auto& sock_queue = socket->get_packets_sent_to_client();
  ASSERT_EQ(2, sock_queue.size());
  sock_queue.pop(); // pop join msg
  std::string data_client_received(std::move(sock_queue.front()));
  sock_queue.pop();
  binary_deserializer bd(data_client_received);
  std::string chatroom, user, msg;
  ingame_state_protocol::to_client_packet_opcodes opcode;
  bd >> opcode >> chatroom >> user >> msg;
  EXPECT_EQ(ingame_state_protocol::to_client_packet_opcodes::chat_msg, opcode);
  EXPECT_EQ("czekoladowa wysepka", chatroom);
  EXPECT_EQ("Krzysztof", user);
  EXPECT_EQ("no eloszki pysiaczki", msg);

}