#pragma once

#include <friend_system_error_codes.hpp>
#include <string>

class friend_backend
{
  public:

    virtual void on_friend_online_while_you_login(const std::string& his_nick) = 0;
    virtual void on_friend_login(const std::string& his_nick) = 0;
    virtual void on_friend_leave(const std::string& his_nick) = 0;
    virtual void on_new_friendship(const std::string& his_nick) = 0;
    virtual void on_broken_friendship(const std::string& his_nick) = 0;
    virtual void on_friendship_request(const std::string& his_nick) = 0;
    virtual void on_my_request_sent(const std::string& his_nick, friend_add_request_result result) = 0;
    virtual void on_friend_add_fail(const std::string& his_nick, friend_add_fail_reason reason) = 0;
    virtual void on_friend_removal_fail(const std::string& his_nick) = 0;

    virtual ~friend_backend() = default;
};
