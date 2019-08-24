#include <binary_deserializer.hpp>
#include <c2s/request_character_create.hpp>
#include <c2s/request_character_delete.hpp>
#include <c2s/request_character_select.hpp>
#include <c2s/request_lobby_log_off.hpp>
#include <lobby_state_protocol.hpp>
#include <lobby_state_recv_packet_factory.hpp>
#include <null_received_packet.hpp>

lobby_state_recv_packet_factory::lobby_state_recv_packet_factory(
  std::shared_ptr<net_session> my_session,
  state_transitioner& transitioner,
  world& universe,
  logger& logger,
  lobby_character::lobby_character_list& lobby_chars,
  database_facade& db,
  account_data& acc_data,
  asynchronous_database_adapter& async_db)
  : my_session_(my_session)
  , transitioner_(transitioner)
  , world_(universe)
  , logger_(logger)
  , lobby_chars_(lobby_chars)
  , db_(db)
  , acc_data_(acc_data)
  , async_db_(async_db)
{
}

received_packet::packet_ptr
lobby_state_recv_packet_factory::construct(const std::array<char, 2048>& data, unsigned size)
{
  binary_deserializer bd(data.data(), size);
  lobby_state_protocol::to_server_packet_opcodes opcode;
  bd >> opcode;
  switch (opcode)
  {
    case lobby_state_protocol::select_character:
      return std::make_unique<request_character_select>(
        bd, my_session_, transitioner_, world_, logger_, lobby_chars_, db_, acc_data_, async_db_);
    case lobby_state_protocol::create_character:
      return std::make_unique<request_character_create>(
        bd, my_session_, logger_, lobby_chars_, db_, acc_data_);
    case lobby_state_protocol::delete_character:
      return std::make_unique<request_character_delete>(
        bd, my_session_, logger_, lobby_chars_, db_);
    case lobby_state_protocol::log_off:
      return std::make_unique<request_lobby_log_off>(my_session_);
  }
  return std::make_unique<null_received_packet>();
}
