#include <friendlist_backend.hpp>

friendlist_backend::friendlist_backend(representative_ref my_char)
  : my_char_(my_char)
{
}

const friendlist_backend::representative_ref friendlist_backend::representative() const
{
  return my_char_;
}

void friendlist_backend::on_friend_online_while_you_login(const std::string& his_nick)
{
  //printf("[%s]\tYour friend %s is already in game\n", my_char_.name().c_str(), his_nick.c_str());
}

void friendlist_backend::on_friend_login(const std::string& his_nick)
{
  //printf("[%s]\tOne of my friends logged in %s\n", my_char_.name().c_str(), his_nick.c_str());
}

void friendlist_backend::on_friend_leave(const std::string& his_nick)
{
  //printf("[%s]\tOne of my friends logged off %s\n", my_char_.name().c_str(), his_nick.c_str());
}

void friendlist_backend::on_new_friendship(const std::string& his_nick)
{
  //printf("[%s]\tNew friendship with %s\n", my_char_.name().c_str(), his_nick.c_str());
}

void friendlist_backend::on_broken_friendship(const std::string& his_nick)
{
  //printf("[%s]\tBye bye %s\n", my_char_.name().c_str(), his_nick.c_str());
}

void friendlist_backend::on_friendship_request(const std::string& his_nick)
{
  //printf("[%s]\t%s asks you to be his friend\n", my_char_.name().c_str(), his_nick.c_str());
}

void friendlist_backend::on_my_request_sent(const std::string& his_nick, friend_add_request_result result)
{
  //albo OK
  //printf("[%s]\t%s still has time to reply at your request! Chill!\n", my_char_.name().c_str(), his_nick.c_str());
}

void friendlist_backend::on_friend_add_fail(const std::string& his_nick, friend_add_fail_reason reason)
{
}

void friendlist_backend::on_friend_removal_fail(const std::string& his_nick)
{
}
