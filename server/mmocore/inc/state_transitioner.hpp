#pragma once

#include <client_state.hpp>
#include <memory>

class state_transitioner
{
  protected:

    std::unique_ptr<client_state> current_state_;

  public:

    void transition(std::unique_ptr<client_state> new_state)
    {
      current_state_ = std::move(new_state);
      current_state_->start();
    }
};