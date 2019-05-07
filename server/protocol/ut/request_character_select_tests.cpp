#include <gtest/gtest.h>
#include <binary_serializer.hpp>
#include <binary_deserializer.hpp>
#include <c2s/request_character_select.hpp>
#include <s2c/character_selected.hpp>
#include <fake_net_session.hpp>
#include <fake_net_socket.hpp>
#include <fake_logger.hpp>
#include <fake_database_facade.hpp>
#include <lobby_state_protocol.hpp>
#include <state_transitioner.hpp>
#include <world.hpp>

TEST(request_character_select, construction)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_char_name");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  state_transitioner transitioner;
  fake_logger log;
  fake_database_facade db;
  world universe;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  EXPECT_NO_THROW(request_character_select(incoming_stream, session, transitioner, universe, log, lobby_chars, db, acc_data));
}

TEST(request_character_select, throw_on_missing_nickname)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << 123;
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  state_transitioner transitioner;
  fake_logger log;
  fake_database_facade db;
  world universe;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  EXPECT_THROW(request_character_select(incoming_stream, session, transitioner, universe, log, lobby_chars, db, acc_data), std::underflow_error);
}

TEST(request_character_select, send_char_selected_packet)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("KoncepcyjnyMiliarder");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  state_transitioner transitioner;
  fake_logger log;
  fake_database_facade db;
  world universe;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  request_character_select(incoming_stream, session, transitioner, universe, log, lobby_chars, db, acc_data).execute_associated_action();

  auto& sock_queue = socket->get_packets_sent_to_client();
  ASSERT_EQ(1, sock_queue.size());

  std::string data_client_received(std::move(sock_queue.front()));
  sock_queue.pop();
  binary_deserializer bd(data_client_received);
  lobby_state_protocol::to_client_packet_opcodes opcode;
  std::string name;
  bd >> opcode >> name;
  EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_selected, opcode);
  EXPECT_EQ("KoncepcyjnyMiliarder", name);
}
