#include <fake_user_environment.hpp>

fake_user_environment::env_session_pair fake_user_environment::make_env(const std::string& account_login, const std::string& character_nickname)
{
  auto socket = std::make_shared<fake_net_socket>();
  auto session = std::make_shared<fake_net_session>(socket);
  account_data acc_data = db.get_account_data(account_login);
  return { std::make_unique<user_environment>(session, unvierse.chat_, character_nickname, logger, unvierse, db, acc_data), session };
}
