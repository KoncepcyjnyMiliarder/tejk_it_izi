#include <binary_deserializer.hpp>
#include <binary_serializer.hpp>
#include <c2s/log_off_to_lobby.hpp>
#include <fake_user_environment.hpp>
#include <gtest/gtest.h>
#include <lobby_state_protocol.hpp>
#include <state_transitioner.hpp>

TEST(log_off_to_lobby, construction)
{
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("mirek", "Tag");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;
  state_transitioner transitioner;

  EXPECT_NO_THROW(log_off_to_lobby(transitioner, my_env));
}

TEST(join_chat, send_charlist_on_execution)
{
  fake_user_environment env;
  auto pair_of_mocks = env.make_env("mirek", "Tag");
  user_environment& my_env = *pair_of_mocks.first;
  std::shared_ptr<fake_net_session> my_session = pair_of_mocks.second;
  state_transitioner transitioner;

  log_off_to_lobby(transitioner, my_env).execute_associated_action();
  // resuls in changing state to lobby_state and sending character list to the
  // client
  {
    auto& sock_queue = my_session->get_sock()->get_packets_sent_to_client();
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);

    std::string chatroom, user;
    lobby_state_protocol::to_client_packet_opcodes opcode;
    std::size_t count;
    bd >> opcode >> count;
    EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_list, opcode);
    auto chars = my_env.db_.get_lobby_chars(my_env.acc_data_.uid);
    ASSERT_EQ(chars.size(), count);

    for (const auto& character : chars)
    {
      unsigned uid;
      std::string nick;
      bd >> uid >> nick;
      EXPECT_EQ(character.uid, uid);
      EXPECT_EQ(character.name, nick);
    }
  }
}
