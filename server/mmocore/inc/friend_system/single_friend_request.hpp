#pragma once

#include <functional>

struct single_friend_request
{
  unsigned request_sender_id;
  unsigned recipient_id;

  single_friend_request(unsigned request_sender_id, unsigned recipient_id)
    : request_sender_id(request_sender_id)
    , recipient_id(recipient_id)
  {
  }

  struct hash
  {
    std::size_t operator()(const single_friend_request& val) const
    {
      auto request_sender_id_hash = std::hash<unsigned>{}(val.request_sender_id);
      auto recipient_id_hash = std::hash<unsigned>{}(val.recipient_id);
      return (request_sender_id_hash * 0x1f1f1f1f) ^ recipient_id_hash; // stackoverflow qq
    }
  };

  struct equal
  {
    bool operator()(const single_friend_request& lhs, const single_friend_request& rhs) const
    {
      return rhs.request_sender_id == lhs.request_sender_id && rhs.recipient_id == lhs.recipient_id;
    }
  };
};