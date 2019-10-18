#pragma once

#include <boost/asio.hpp>
#include <memory>

class task_scheduler
{

  boost::asio::strand<boost::asio::io_context::executor_type>& sync_strand_;

public:
  class postponed_task
  {
    boost::asio::deadline_timer timer_to_action_;

  public:
    template <typename Task>
    postponed_task(const boost::posix_time::time_duration& time_from_now,
                   boost::asio::strand<boost::asio::io_context::executor_type>& sync_strand_,
                   Task&& task)
      : timer_to_action_(sync_strand_, time_from_now)
    {
      // after moving to boost 1.70 i am constructing the timer directly from strand and not
      // io_service but still i am binding executor to strand again there. is that all ok?
      timer_to_action_.async_wait(
        boost::asio::bind_executor(sync_strand_, std::forward<Task&&>(task)));
    }

    void cancel() { timer_to_action_.cancel(); }
  };

  task_scheduler(boost::asio::strand<boost::asio::io_context::executor_type>& sync_strand)
    : sync_strand_(sync_strand)
  {
  }

  template <typename Task>
  void schedule_task(Task&& task)
  {
    boost::asio::post(sync_strand_, std::forward<Task&&>(task));
  }

  template <typename Task>
  std::unique_ptr<postponed_task> postpone_task(
    Task&& task,
    const boost::posix_time::time_duration& time_from_now)
  {
    return std::make_unique<postponed_task>(
      time_from_now, sync_strand_, std::forward<Task&&>(task));
  }
};
