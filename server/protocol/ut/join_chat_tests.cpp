#include <gtest/gtest.h>
#include <binary_serializer.hpp>
#include <c2s/join_chat.hpp>
#include <fake_net_session.hpp>
#include <fake_net_socket.hpp>
#include <fake_logger.hpp>
#include <ingame_state_protocol.hpp>

TEST(join_chat, construction)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  world universe;
  world::player_pawn my_pawn(universe, session, "Krzysztof");
  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in;
  EXPECT_NO_THROW(join_chat(incoming_stream, session, log, my_pawn, universe.chat_, chatrooms_im_in));
}

TEST(join_chat, throw_on_packet_underflow)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << 7; //will cause underflow
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  world universe;
  world::player_pawn my_pawn(universe, session, "Krzysztof");
  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in;
  EXPECT_THROW(join_chat(incoming_stream, session, log, my_pawn, universe.chat_, chatrooms_im_in), std::underflow_error);
}

TEST(join_chat, world_state_after_executing_packet)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name") << std::string("some_other_room");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  world universe;
  world::player_pawn my_pawn(universe, session, "Krzysztof");
  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in;
  auto& sock_queue = socket->get_packets_sent_to_client();
  //user sent join chat request
  join_chat(incoming_stream, session, log, my_pawn, universe.chat_, chatrooms_im_in).execute_associated_action();
  {
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);
    std::string chatroom, user;
    ingame_state_protocol::to_client_packet_opcodes opcode;
    bd >> opcode >> chatroom >> user;
    EXPECT_EQ(ingame_state_protocol::to_client_packet_opcodes::user_joined_chat, opcode);
    EXPECT_EQ("some_room_name", chatroom);
    EXPECT_EQ("Krzysztof", user);
    EXPECT_EQ(1, chatrooms_im_in.size());
  }
  //sent again
  join_chat(incoming_stream, session, log, my_pawn, universe.chat_, chatrooms_im_in).execute_associated_action();
  {
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);
    std::string chatroom, user;
    ingame_state_protocol::to_client_packet_opcodes opcode;
    bd >> opcode >> chatroom >> user;
    EXPECT_EQ(ingame_state_protocol::to_client_packet_opcodes::user_joined_chat, opcode);
    EXPECT_EQ("some_other_room", chatroom);
    EXPECT_EQ("Krzysztof", user);
    EXPECT_EQ(2, chatrooms_im_in.size());
  }
}