#pragma once

#include <memory>

// todo think of better name and refactor later
struct gameserver_write_base
{
  virtual void request_kick_player(const std::string& name) = 0;
  virtual ~gameserver_write_base() = default;
};

struct gameserver
{
  // Gameserver keeps connection with loginserver to keep track of users logging in and off.
  // Sadly, this connection can be broken (and hopefully reestabilished later)
  // In such case we assume that every account is still logged in on the GS
  std::weak_ptr<gameserver_write_base> associated_gameserver_session;
};
