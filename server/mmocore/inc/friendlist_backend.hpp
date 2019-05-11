#pragma once

#include <tii_player_character.hpp>
#include <friend_system_error_codes.hpp>

class friendlist_backend
{
  public:

    using representative_ref = typename tii_player_character::reference;

  private:

    representative_ref my_char_;

  public:

    friendlist_backend(representative_ref my_char);

    const representative_ref representative() const;
    void on_friend_online_while_you_login(const std::string& his_nick);
    void on_friend_login(const std::string& his_nick);
    void on_friend_leave(const std::string& his_nick);
    void on_new_friendship(const std::string& his_nick);
    void on_broken_friendship(const std::string& his_nick);
    void on_friendship_request(const std::string& his_nick);
    void on_my_request_sent(const std::string& his_nick, friend_add_request_result result);
    void on_friend_add_fail(const std::string& his_nick, friend_add_fail_reason reason);
    void on_friend_removal_fail(const std::string& his_nick);
};
