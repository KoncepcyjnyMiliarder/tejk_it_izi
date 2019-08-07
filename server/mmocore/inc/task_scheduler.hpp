#pragma once

#include <boost/asio.hpp>

class task_scheduler
{

    boost::asio::strand& sync_strand_;

  public:

    task_scheduler(boost::asio::strand& sync_strand)
      : sync_strand_(sync_strand)
    {}

    template<typename Task>
    void schedule_task(Task&& task)
    {
      sync_strand_.post(std::forward < Task && > (task));
    }
};