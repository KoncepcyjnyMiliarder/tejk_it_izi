#pragma once

#include <received_packet.hpp>
#include <net_session.hpp>

class request_lobby_log_off
  : public received_packet
{
    std::shared_ptr<net_session> my_session_;

  public:

    request_lobby_log_off(std::shared_ptr<net_session> my_session);
    virtual void execute_associated_action() override;
};