#include <gtest/gtest.h>
#include <binary_serializer.hpp>
#include <c2s/leave_chat.hpp>
#include <c2s/join_chat.hpp>
#include <fake_net_session.hpp>
#include <fake_net_socket.hpp>
#include <fake_logger.hpp>
#include <fake_database_facade.hpp>
#include <ingame_state_protocol.hpp>

TEST(leave_chat, construction)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  fake_logger log;
  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in;
  EXPECT_NO_THROW(leave_chat(incoming_stream, log, chatrooms_im_in));
}

TEST(leave_chat, throw_on_packet_underflow)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << 5; //underflow
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  fake_logger log;
  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in;
  EXPECT_THROW(leave_chat(incoming_stream, log, chatrooms_im_in), std::underflow_error);
}

TEST(leave_chat, world_state_after_executing_packet)
{
  world universe;
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  world::player_pawn my_pawn(universe, session, "Krzysztof");
  std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in;
  auto& sock_queue = socket->get_packets_sent_to_client();
  //first, let's join 2 chatrooms
  {
    net_socket::buffer buf;
    binary_serializer bs(buf);
    bs << std::string("some_room_name") << std::string("some_other_room");
    binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
    join_chat(incoming_stream, session, log, my_pawn, universe.chat_, chatrooms_im_in).execute_associated_action();
    join_chat(incoming_stream, session, log, my_pawn, universe.chat_, chatrooms_im_in).execute_associated_action();
    sock_queue.pop();
    sock_queue.pop();
  }
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name") << std::string("some_other_room");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  //leave "some_room_name"
  leave_chat(incoming_stream, log, chatrooms_im_in).execute_associated_action();
  {
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);
    std::string chatroom, user;
    ingame_state_protocol::to_client_packet_opcodes opcode;
    bd >> opcode >> chatroom >> user;
    EXPECT_EQ(ingame_state_protocol::to_client_packet_opcodes::user_left_chat, opcode);
    EXPECT_EQ("some_room_name", chatroom);
    EXPECT_EQ("Krzysztof", user);
    EXPECT_EQ(1, chatrooms_im_in.size());
  }
  //leave "some_other_room"
  leave_chat(incoming_stream, log, chatrooms_im_in).execute_associated_action();
  {
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);
    std::string chatroom, user;
    ingame_state_protocol::to_client_packet_opcodes opcode;
    bd >> opcode >> chatroom >> user;
    EXPECT_EQ(ingame_state_protocol::to_client_packet_opcodes::user_left_chat, opcode);
    EXPECT_EQ("some_other_room", chatroom);
    EXPECT_EQ("Krzysztof", user);
    EXPECT_EQ(0, chatrooms_im_in.size());
  }
}