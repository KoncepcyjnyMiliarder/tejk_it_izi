#pragma once

#include <logger.hpp>
#include <queue>

class fake_logger
  : public logger
{

    std::queue<std::string> log_msgs_;

  public:

    virtual void log(const std::string& msg) override;
    std::queue<std::string>& get_log();
};