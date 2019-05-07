#include <gtest/gtest.h>
#include <binary_serializer.hpp>
#include <binary_deserializer.hpp>
#include <c2s/request_character_delete.hpp>
#include <fake_net_session.hpp>
#include <fake_net_socket.hpp>
#include <fake_logger.hpp>
#include <fake_database_facade.hpp>
#include <lobby_state_protocol.hpp>

TEST(request_character_delete, construction)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_char_name");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  fake_database_facade db;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  EXPECT_NO_THROW(request_character_delete(incoming_stream, session, log, lobby_chars, db));
}

TEST(request_character_delete, throw_on_missing_nickname)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << 123456; //underflow
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  fake_database_facade db;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  EXPECT_THROW(request_character_delete(incoming_stream, session, log, lobby_chars, db), std::underflow_error);
}

TEST(request_character_delete, disconnect_session_on_wrong_nickname_request)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("some_char_name");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  fake_database_facade db;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  request_character_delete(incoming_stream, session, log, lobby_chars, db).execute_associated_action();
  EXPECT_FALSE(socket->is_open());
}

TEST(request_character_delete, resend_charlist_on_successful_removal)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("KoncepcyjnyMiliarder");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  fake_database_facade db;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  request_character_delete(incoming_stream, session, log, lobby_chars, db).execute_associated_action();

  auto& sock_queue = socket->get_packets_sent_to_client();
  ASSERT_EQ(1, sock_queue.size());
  std::string data_client_received(std::move(sock_queue.front()));
  sock_queue.pop();
  binary_deserializer bd(data_client_received);

  lobby_state_protocol::to_client_packet_opcodes opcode;
  std::size_t count;
  bd >> opcode >> count;
  EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_list, opcode);
  auto chars = db.get_lobby_chars(acc_data.uid);
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
