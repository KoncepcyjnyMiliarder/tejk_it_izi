#pragma once

#include <received_packet.hpp>
#include <state_transitioner.hpp>
#include <user_environment.hpp>

class log_off_to_lobby : public received_packet
{
  state_transitioner& transitioner_;
  user_environment& my_environment_;

public:
  log_off_to_lobby(state_transitioner& transitioner, user_environment& my_environment);

  virtual void execute_associated_action() override;
};