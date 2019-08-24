#include <algorithm>
#include <fake_user_environment.hpp>

fake_user_environment::fake_user_environment()
  : unvierse(db)
{
}

fake_user_environment::env_session_pair
fake_user_environment::make_env(const std::string& account_login,
                                const std::string& character_nickname)
{
  auto socket = std::make_shared<fake_net_socket>();
  auto session = std::make_shared<fake_net_session>(socket);
  account_data acc_data = db.get_account_data(account_login);
  auto lobby_chars = db.get_lobby_chars(acc_data.uid);
  auto char_iter = std::find_if(
    lobby_chars.begin(), lobby_chars.end(), [&character_nickname](const lobby_character& lc) {
      return lc.name == character_nickname;
    });
  if (char_iter == lobby_chars.end())
    throw std::invalid_argument("You requested to enter world with character "
                                "nonexistent in database: " +
                                account_login + '/' + character_nickname);
  /*
  user_environment(std::shared_ptr<net_session> my_session, world& universe,
  unsigned my_id, const std::string& name,
                   logger& logger,  database_facade& db, account_data
  acc_data)*/
  return {std::make_unique<user_environment>(session, unvierse, *char_iter, logger, db, acc_data),
          session};
}
