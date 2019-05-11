#include <gtest/gtest.h>
#include <binary_serializer.hpp>
#include <binary_deserializer.hpp>
#include <c2s/request_character_create.hpp>
#include <s2c/character_created.hpp>
#include <s2c/character_creation_fail.hpp>
#include <fake_net_session.hpp>
#include <fake_net_socket.hpp>
#include <fake_logger.hpp>
#include <fake_database_facade.hpp>
#include <lobby_state_protocol.hpp>

TEST(request_character_create, construction)
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
  EXPECT_NO_THROW(request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data));
}

TEST(request_character_create, throw_on_missing_nickname)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << 123; //underflow
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  fake_database_facade db;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  EXPECT_THROW(request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data), std::underflow_error);
}

TEST(request_character_create, limit_of_5_characters)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  std::string char_names[] = { "ala", "ma", "kota", "fail" };
  for (auto& elem : char_names)
    bs << elem;
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  fake_database_facade db;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();

  auto& sock_queue = socket->get_packets_sent_to_client();
  ASSERT_EQ(4, sock_queue.size());
  for (int i = 0; i < 3; ++i)
  {
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);
    lobby_state_protocol::to_client_packet_opcodes opcode;
    unsigned uid;
    std::string name;
    bd >> opcode >> uid >> name;
    EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_created, opcode);
    EXPECT_EQ(7 + i, uid);
    EXPECT_EQ(char_names[i], name);
  }
  std::string data_client_received(std::move(sock_queue.front()));
  sock_queue.pop();
  binary_deserializer bd(data_client_received);
  lobby_state_protocol::to_client_packet_opcodes opcode;
  character_creation_fail_reason fail_reason;
  bd >> opcode >> fail_reason;
  EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_creation_fail, opcode);
  EXPECT_EQ(character_creation_fail_reason::character_limit_exceeded, fail_reason);
  EXPECT_EQ(5, db.get_lobby_chars(acc_data.uid).size());
  EXPECT_EQ(5, lobby_chars.size());
}

TEST(request_character_create, fail_on_non_alphanumeric_chars)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("cat\'dog") << std::string("cat#dog") << std::string("cat_dog") << std::string("cat|dog") << std::string("cat?dog");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  fake_database_facade db;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();

  auto& sock_queue = socket->get_packets_sent_to_client();
  ASSERT_EQ(5, sock_queue.size());
  for(int i = 0; i < 5; ++i)
  {
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);

    lobby_state_protocol::to_client_packet_opcodes opcode;
    character_creation_fail_reason fail_reason;
    bd >> opcode >> fail_reason;
    EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_creation_fail, opcode);
    EXPECT_EQ(character_creation_fail_reason::name_contains_incorrect_digits, fail_reason);
  }
}

TEST(request_character_create, fail_on_already_taken_nickname)
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
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  {
    auto& sock_queue = socket->get_packets_sent_to_client();
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);

    lobby_state_protocol::to_client_packet_opcodes opcode;
    character_creation_fail_reason fail_reason;
    bd >> opcode >> fail_reason;
    EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_creation_fail, opcode);
    EXPECT_EQ(character_creation_fail_reason::name_already_used, fail_reason);
  }
}

TEST(request_character_create, on_too_long_nick_dont_create_char)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("thisNicknameIsOf21Len");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  fake_database_facade db;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  {
    auto& sock_queue = socket->get_packets_sent_to_client();
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);

    lobby_state_protocol::to_client_packet_opcodes opcode;
    character_creation_fail_reason fail_reason;
    bd >> opcode >> fail_reason;
    EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_creation_fail, opcode);
    EXPECT_EQ(character_creation_fail_reason::name_too_long, fail_reason);
  }
}

TEST(request_character_create, on_empty_nick_dont_create_char)
{
  net_socket::buffer buf;
  binary_serializer bs(buf);
  bs << std::string("");
  binary_deserializer incoming_stream(buf.data(), bs.get_current_size());
  auto socket = std::make_shared<fake_net_socket>();
  auto session(std::make_shared<fake_net_session>(socket));
  fake_logger log;
  fake_database_facade db;
  account_data acc_data = db.get_account_data("krzysztof");
  lobby_character::lobby_character_list lobby_chars = db.get_lobby_chars(acc_data.uid);
  request_character_create(incoming_stream, session, log, lobby_chars, db, acc_data).execute_associated_action();
  {
    auto& sock_queue = socket->get_packets_sent_to_client();
    ASSERT_EQ(1, sock_queue.size());
    std::string data_client_received(std::move(sock_queue.front()));
    sock_queue.pop();
    binary_deserializer bd(data_client_received);

    lobby_state_protocol::to_client_packet_opcodes opcode;
    character_creation_fail_reason fail_reason;
    bd >> opcode >> fail_reason;
    EXPECT_EQ(lobby_state_protocol::to_client_packet_opcodes::character_creation_fail, opcode);
    EXPECT_EQ(character_creation_fail_reason::name_too_long, fail_reason);
  }
}
