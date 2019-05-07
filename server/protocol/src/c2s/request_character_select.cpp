#include <c2s/request_character_select.hpp>
#include <s2c/character_selected.hpp>
#include <ingame_state.hpp>

request_character_select::request_character_select(binary_deserializer& bd, std::shared_ptr<net_session> my_session, state_transitioner& transitioner,
    world& universe, logger& logger, lobby_character::lobby_character_list& lobby_chars, database_facade& db, account_data& acc_data)
  : my_session_(my_session),
    transitioner_(transitioner),
    world_(universe),
    logger_(logger),
    lobby_chars_(lobby_chars),
    db_(db),
    acc_data_(acc_data)
{
  bd >> char_name_;
}

void request_character_select::execute_associated_action()
{
  auto it = std::find_if(lobby_chars_.begin(), lobby_chars_.end(), [this](const lobby_character & character)
  {
    return character.name == char_name_;
  });

  if (it == lobby_chars_.end())
  {
    logger_.log("someone requested to join game with nonexistent character: " + char_name_);
    my_session_->force_close();
    return;
  }

  std::array<char, 2048> buffer;
  auto len = lobby_packet_constructors::character_selected(buffer, *it);
  my_session_->send_to_client(buffer, len);
  transitioner_.transition(std::make_unique<ingame_state>(transitioner_, my_session_, world_, logger_, it->name, db_, acc_data_)); //C A R E F U L
}
