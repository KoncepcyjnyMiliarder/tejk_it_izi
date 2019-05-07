#include <gtest/gtest.h>
#include <binary_serializer.hpp>
#include <binary_deserializer.hpp>
#include <c2s/log_off_to_lobby.hpp>
#include <fake_net_session.hpp>
#include <fake_net_socket.hpp>
#include <fake_logger.hpp>
#include <fake_database_facade.hpp>
#include <lobby_state_protocol.hpp>

TEST(log_off_to_lobby, construction)
{
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  state_transitioner transitioner;
  world universe;
  fake_database_facade db;
  account_data acc_data(db.get_account_data("mirek"));
  EXPECT_NO_THROW(log_off_to_lobby(session, log, transitioner, universe, db, acc_data));
}

TEST(join_chat, send_charlist_on_execution)
{
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  state_transitioner transitioner;
  world universe;
  fake_database_facade db;
  account_data acc_data(db.get_account_data("mirek"));
  log_off_to_lobby(session, log, transitioner, universe, db, acc_data).execute_associated_action();
  //resuls in changing state to lobby_state and sending character list to the client
  {
    auto& sock_queue = socket->get_packets_sent_to_client();
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);

    std::string chatroom, user;
    lobby_state_protocol::to_client_packet_opcodes opcode;
    std::size_t count;
    bd >> opcode >> count;
    EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_list, opcode);
    auto chars = db.get_lobby_chars(acc_data.uid);
    ASSERT_EQ(chars.size(), count);

    for(const auto& character : chars)
    {
      unsigned uid;
      std::string nick;
      bd >> uid >> nick;
      EXPECT_EQ(character.uid, uid);
      EXPECT_EQ(character.name, nick);
    }
  }
}
