#include <gtest/gtest.h>
#include <binary_serializer.hpp>
#include <c2s/join_chat.hpp>
#include <c2s/send_chat_msg.hpp>
#include <fake_user_environment.hpp>
#include <ingame_state_protocol.hpp>

TEST(send_chat_msg, construction)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("czekoladowa wysepka") << std::string("no eloszki pysiaczki");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("krzysztof", "KoncepcyjnyMiliarder");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;

  EXPECT_NO_THROW(send_chat_msg(incoming_stream, my_env));
}

TEST(send_chat_msg, throw_on_packet_underflow)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_room_name"); // didnt send message
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("krzysztof", "KoncepcyjnyMiliarder");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;

  EXPECT_THROW(send_chat_msg(incoming_stream, my_env), std::underflow_error);
}

TEST(send_chat_msg, send_message_to_all_participants)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("czekoladowa wysepka") << std::string("no eloszki pysiaczki");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());

  fake_user_environment env;

  auto my_pair_of_mocks = env.make_env("krzysztof", "KoncepcyjnyMiliarder");
  user_environment& my_env = *my_pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = my_pair_of_mocks.second;

  auto other_pair_of_mocks = env.make_env("foo", "foobar");
  user_environment& other_env = *other_pair_of_mocks.first;
  std::shared_ptr<fake_net_session> other_session = other_pair_of_mocks.second;

  const std::string room_name = "czekoladowa wysepka";
  other_env.chat_controller_.register_me(room_name);
  my_env.chat_controller_.register_me(room_name);

  send_chat_msg(incoming_stream, my_env).execute_associated_action();
  auto& sock_queue = my_session->get_sock()->get_packets_sent_to_client();
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
  EXPECT_EQ("KoncepcyjnyMiliarder", user);
  EXPECT_EQ("no eloszki pysiaczki", msg);

}