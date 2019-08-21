#pragma once

#include <friend_system/single_friend_request.hpp>
#include <unordered_map>
#include <chrono>
#include <task_scheduler.hpp>

class pending_friend_requests
{

    using request_hashmap = std::unordered_map<single_friend_request, std::chrono::system_clock::time_point,
          single_friend_request::hash, single_friend_request::equal>;

    task_scheduler& scheduler_;
    std::unique_ptr<task_scheduler::postponed_task> periodic_cleanup_task_;
    request_hashmap pending_friend_requests_front_;
    request_hashmap pending_friend_requests_back_;

    void periodic_cleanup();

  public:

    enum operation_result
    {
      already_in_buffer,
      succesfully_added,
      that_was_a_match
    };

    pending_friend_requests(task_scheduler& scheduler);
    operation_result process_add_friend_request(unsigned sender, unsigned recipient, std::chrono::system_clock::time_point current_time);
};