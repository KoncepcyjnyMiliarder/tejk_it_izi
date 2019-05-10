#pragma once

enum class friend_add_fail_reason
{
  player_not_online,
  cannot_invite_self,
  already_friends
};

enum class friend_add_request_result
{
  sent_succesfully,
  previous_still_waiting
};