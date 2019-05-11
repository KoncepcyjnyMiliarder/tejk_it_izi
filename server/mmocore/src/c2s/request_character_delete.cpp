#include <c2s/request_character_delete.hpp>
#include <s2c/character_list.hpp>

request_character_delete::request_character_delete(binary_deserializer& bd, std::shared_ptr<net_session> my_session,
    logger& logger, lobby_character::lobby_character_list& lobby_chars, database_facade& db)
  : my_session_(my_session),
    lobby_chars_(lobby_chars),
    logger_(logger),
    db_(db)
{
  bd >> char_name_;
}

void request_character_delete::execute_associated_action()
{
  auto it = std::find_if(lobby_chars_.begin(), lobby_chars_.end(), [this](const lobby_character & character)
  {
    return character.name == char_name_;
  });

  if (it == lobby_chars_.end())
  {
    logger_.log("someone requested to delete nonexistent character: " + char_name_);
    my_session_->force_close();
    return;
  }

  db_.erase_character(*it);
  lobby_chars_.erase(it);
  std::array<char, 2048> buffer;
  auto len = lobby_packet_constructors::character_list(buffer, lobby_chars_);
  my_session_->send_to_client(buffer, len);
}
