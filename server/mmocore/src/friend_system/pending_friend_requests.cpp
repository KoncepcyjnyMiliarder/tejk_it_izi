#include <friend_system/pending_friend_requests.hpp>

void
pending_friend_requests::periodic_cleanup()
{
  // THIS IS SO BEAUTIFUL
  pending_friend_requests_back_.clear();
  std::swap(pending_friend_requests_front_, pending_friend_requests_back_);
  periodic_cleanup_task_ = std::move(scheduler_.postpone_task(
    std::bind(&pending_friend_requests::periodic_cleanup, this), boost::posix_time::seconds(20)));
}

pending_friend_requests::pending_friend_requests(task_scheduler& scheduler)
  : scheduler_(scheduler)
{
  periodic_cleanup_task_ = std::move(scheduler_.postpone_task(
    std::bind(&pending_friend_requests::periodic_cleanup, this), boost::posix_time::seconds(20)));
}

pending_friend_requests::operation_result
pending_friend_requests::process_add_friend_request(
  unsigned sender,
  unsigned recipient,
  std::chrono::system_clock::time_point current_time)
{
  // if the request from recipient to sender is in front buffer, match
  single_friend_request this_request_reverse(recipient, sender);

  auto front_iter = pending_friend_requests_front_.find(this_request_reverse);
  if (front_iter != pending_friend_requests_front_.end())
  {
    pending_friend_requests_front_.erase(front_iter);
    return operation_result::that_was_a_match;
  }

  // if the request is in back buffer, additionally confirm timestamp
  auto back_iter = pending_friend_requests_back_.find(this_request_reverse);
  if (back_iter != pending_friend_requests_back_.end())
  {
    auto seconds =
      std::chrono::duration_cast<std::chrono::seconds>(current_time - back_iter->second);
    if (seconds.count() < 20)
    {
      pending_friend_requests_back_.erase(back_iter);
      return operation_result::that_was_a_match;
    }
  }

  // turns out it was not a response to a request
  // maybe the user has sent the request just before a moment?
  single_friend_request this_request(sender, recipient);

  front_iter = pending_friend_requests_front_.find(this_request);
  if (front_iter != pending_friend_requests_front_.end())
  {
    pending_friend_requests_front_.erase(front_iter);
    return operation_result::already_in_buffer;
  }

  back_iter = pending_friend_requests_back_.find(this_request);
  if (back_iter != pending_friend_requests_back_.end())
  {
    auto seconds =
      std::chrono::duration_cast<std::chrono::seconds>(current_time - back_iter->second);
    if (seconds.count() < 20)
    {
      pending_friend_requests_back_.erase(back_iter);
      return operation_result::already_in_buffer;
    }
  }

  // so now, we got to add the request to front buffer
  pending_friend_requests_front_.emplace(this_request, current_time);
  return operation_result::succesfully_added;
}
