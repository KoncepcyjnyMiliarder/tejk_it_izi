#pragma once

#include <client_state.hpp>
#include <memory>

class state_transitioner
{

  protected:

    std::unique_ptr<client_state> state_awaiting_transition_;
    std::unique_ptr<client_state> current_state_;

  public:

    //yes, not ideally public but only the xxx_state have access to it, so... whatever, maybe think of smth better later
    enum class async_operation_status
    {
      awaiting_packet_poll,
      awaiting_state_transition,
      awaiting_database_operation
    };
    async_operation_status async_state_;
    unsigned count_of_awaiting_database_tasks_;

    state_transitioner()
      : count_of_awaiting_database_tasks_(0)
    {}

    virtual void schedule_transition(std::unique_ptr<client_state> new_state) = 0;
};