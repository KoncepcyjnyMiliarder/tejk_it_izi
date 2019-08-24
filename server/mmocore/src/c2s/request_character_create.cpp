#include <c2s/request_character_create.hpp>
#include <s2c/character_created.hpp>
#include <s2c/character_creation_fail.hpp>

request_character_create::request_character_create(
  binary_deserializer& bd,
  std::shared_ptr<net_session> my_session,
  logger& logger,
  lobby_character::lobby_character_list& lobby_chars,
  database_facade& db,
  account_data& acc_data)
  : my_session_(my_session)
  , lobby_chars_(lobby_chars)
  , logger_(logger)
  , db_(db)
  , acc_data_(acc_data)
{
  bd >> char_name_;
}

void
request_character_create::execute_associated_action()
{
  std::array<char, 2048> buffer;
  if (lobby_chars_.size() >= 5)
  {
    auto len = lobby_packet_constructors::character_creation_fail(
      buffer, character_creation_fail_reason::character_limit_exceeded);
    my_session_->send_to_client(buffer, len);
    return;
  }
  if (char_name_.size() > 20 || char_name_.empty())
  {
    auto len = lobby_packet_constructors::character_creation_fail(
      buffer, character_creation_fail_reason::name_too_long);
    my_session_->send_to_client(buffer, len);
    return;
  }
  if (std::any_of(
        char_name_.begin(), char_name_.end(), [](char c) { return !isalnum((unsigned char)c); }))
  {
    auto len = lobby_packet_constructors::character_creation_fail(
      buffer, character_creation_fail_reason::name_contains_incorrect_digits);
    my_session_->send_to_client(buffer, len);
    return;
  }
  if (db_.is_nickname_already_taken(char_name_))
  {
    auto len = lobby_packet_constructors::character_creation_fail(
      buffer, character_creation_fail_reason::name_already_used);
    my_session_->send_to_client(buffer, len);
    return;
  }
  lobby_chars_.push_back(std::move(db_.create_character(char_name_, acc_data_.uid)));
  auto len = lobby_packet_constructors::character_created(buffer, lobby_chars_.back());
  my_session_->send_to_client(buffer, len);
}
