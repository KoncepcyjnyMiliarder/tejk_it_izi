#include <gtest/gtest.h>
#include <binary_serializer.hpp>
#include <c2s/leave_chat.hpp>
#include <c2s/join_chat.hpp>
#include <fake_user_environment.hpp>
#include <ingame_state_protocol.hpp>

TEST(leave_chat, construction)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("krzysztof", "KoncepcyjnyMiliarder");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;

  EXPECT_NO_THROW(leave_chat(incoming_stream, my_env));
}

TEST(leave_chat, throw_on_packet_underflow)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << 123;
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("krzysztof", "KoncepcyjnyMiliarder");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;

  EXPECT_THROW(leave_chat(incoming_stream, my_env), std::underflow_error);
}

TEST(leave_chat, world_state_after_executing_packet)
{
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("krzysztof", "KoncepcyjnyMiliarder");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;
  auto& sock_queue = my_session->get_sock()->get_packets_sent_to_client();

  //first, let's join 2 chatrooms
  {
    net_socket::buffer buf;
    binary_serializer bs(buf);
    bs << std::string("some_room_name") << std::string("some_other_room");
    binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
    join_chat(incoming_stream, my_env).execute_associated_action();
    join_chat(incoming_stream, my_env).execute_associated_action();
    ASSERT_EQ(2, sock_queue.size());
    sock_queue.pop();
    sock_queue.pop();
  }
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name") << std::string("some_other_room");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  //leave "some_room_name"
  leave_chat(incoming_stream, my_env).execute_associated_action();
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
    EXPECT_EQ("KoncepcyjnyMiliarder", user);
    EXPECT_EQ(1, my_env.chat_controller_.count_of_chatrooms_im_in());
  }
  //leave "some_other_room"
  leave_chat(incoming_stream, my_env).execute_associated_action();
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
    EXPECT_EQ("KoncepcyjnyMiliarder", user);
    EXPECT_EQ(0, my_env.chat_controller_.count_of_chatrooms_im_in());
  }
}