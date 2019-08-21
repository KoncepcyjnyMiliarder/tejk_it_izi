#pragma once

#include <net_session.hpp>
#include <friend_system/friend_backend.hpp>

class default_friend_backend
  : public friend_backend
{
  private:

    const net_session::session_ptr my_session_;

  public:

    default_friend_backend(net_session::session_ptr my_session);

    virtual void on_friend_online_while_you_login(const std::string& his_nick) override;
    virtual void on_friend_login(const std::string& his_nick) override;
    virtual void on_friend_leave(const std::string& his_nick) override;
    virtual void on_new_friendship(const std::string& his_nick) override;
    virtual void on_broken_friendship(const std::string& his_nick) override;
    virtual void on_friendship_request(const std::string& his_nick) override;
    virtual void on_my_request_sent(const std::string& his_nick, friend_add_request_result result) override;
    virtual void on_friend_add_fail(const std::string& his_nick, friend_add_fail_reason reason) override;
    virtual void on_friend_removal_fail(const std::string& his_nick) override;
};
